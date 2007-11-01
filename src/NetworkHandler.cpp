/*
	SiecHandler.cpp
	Ten plik jest częscią kodu źródłowego BeGadu.
	Homepage: http://gadu.beos.pl
*/

//#include <OS.h>
//#include <Application.h>
//#include <stdio.h>
#include <string.h>
//#include <sys/socket.h>
//#include <errno.h>
#include <Message.h>
#include <Alert.h>
#include <ListView.h>
//#include <OutlineListView.h>
#include <String.h>
#include <Roster.h>
//#include <Path.h>
#include <libgadu.h>

#include "Main.h"
#include "Msg.h"
#include "NetworkHandler.h"
#include "Network.h"
#include "Person.h"
#include "GaduListItem.h"
#include "globals.h"

static time_t 	curTime = 0;
static time_t 	pingTimer = 0;

static int Expired( time_t timer );
static void Rearm( time_t* timer, int seconds );

int32 HandlerThread( void *_handler )
	{
	fprintf( stderr, "HandlerThread()\n" );
	NetworkHandler* handler = ( NetworkHandler* ) _handler;
	Network* network = handler->GetNetwork();
	fd_set	rd, wr;
	struct timeval tv;
	if( handler->iDie )
		{
		return -1;
		}
		
	if( !( network->iSession = gg_login( &network->iLoginParam ) ) )
		{
		if( network->iSession )
			{
			fprintf( stderr, "Error creating session: %d", network->iSession->error );
			}
	//	BMessenger( network ).SendMessage( DEL_HANDLER );
		network->Logout();
		return 0;
		}
	else
		{
		gg_change_status( network->Session(), network->iStatus );
		if( network->iWindow )
			{
			BMessenger( network->iWindow ).SendMessage( UPDATE_STATUS );
			BMessage* message = new BMessage( BGDESKBAR_CHSTATE );
			message->AddInt16( "iStatus", network->iStatus );
			BMessenger( network->iWindow ).SendMessage( message );
			// XXX this is here to notify that sync login is complete!
			handler->HandleEventConnected(NULL);
			delete message;
			}
		time( &pingTimer );
		while( !handler->iDie )
			{
			time( &curTime );
			FD_ZERO( &rd );
			FD_ZERO( &wr );
	
			if( ( network->Session()->check & GG_CHECK_READ ) )
				FD_SET( network->Session()->fd, &rd );

			if( ( network->Session()->check & GG_CHECK_WRITE ) )
				FD_SET( network->Session()->fd, &wr );

			tv.tv_sec = 10;
			tv.tv_usec = 0;

			if( select( network->Session()->fd +1, &rd, &wr, NULL, &tv ) < 0 && errno == EBADF )
				{
				fprintf( stderr, "HandlerThread: select() failed (%s)\n", strerror( errno ) );
				BMessenger( network ).SendMessage( DO_LOGOUT );
				break;
				}
			if( handler->iDie )
				break;
			if( network->Session() && ( FD_ISSET( network->Session()->fd, &rd ) ||
										FD_ISSET( network->Session()->fd, &wr ) ) )
				{
				if( !( network->iEvent = gg_watch_fd( network->Session() ) ) )
					perror( "Connection lost." );
				else
					handler->HandleEvent( network->iEvent );
				}
			if( Expired( pingTimer ) )
				handler->HandlePingTimeoutCallback( pingTimer );
			}
		}
	return 0;
	}

NetworkHandler::NetworkHandler( Network* aNetwork, int id, int fd, int cond, void *data )
	{
	fprintf( stderr, "NetworkHandler::NetworkHandler()\n" );
	iNetwork = aNetwork;
	iId		= id;
	iFd		= fd;
	iCond	= cond;
	iData	= data;
	}

void NetworkHandler::Run()
	{
	BMessenger( iNetwork->iWindow ).SendMessage( SET_CONN_DESCRIPTION );

	fprintf( stderr, "NetworkHandler::Run()\n" );
	iDie = false;
	iThreadID = spawn_thread( HandlerThread, "handler thread", B_NORMAL_PRIORITY, this );
	resume_thread( iThreadID );
	}

void NetworkHandler::Stop()
	{
	fprintf( stderr, "NetworkHandler::Stop()\n" );
	status_t exit_value;
	iDie = true;
	wait_for_thread( iThreadID, &exit_value );
	}

void NetworkHandler::HandleEvent( struct gg_event *event )
	{
	fprintf(stderr, "got event %i\n",event->type);
	BMessenger( iNetwork->iWindow ).SendMessage( SET_CONN_DESCRIPTION );
	switch( event->type)
		{
		case GG_EVENT_NONE:
		case GG_EVENT_PONG:
			{
			// fprintf(stderr, "GG_EVENT_NONE || GG_EVENT_PONG\n");
			break;
			}
		case GG_EVENT_CONN_SUCCESS:
			{
			fprintf(stderr,"GG_EVENT_CONN_SUCCESS\n");
			HandleEventConnected( event );
			break;
			}
			
		case GG_EVENT_CONN_FAILED:
			{
			HandleEventConnFailed( event );
			break;
			}
		
		case GG_EVENT_DISCONNECT:
			{
			HandleEventDisconnected( event );
			break;
			}
			
		case GG_EVENT_MSG:
			{
			HandleEventMsg( event );
			break;
			}
			
		case GG_EVENT_USERLIST:
			{
			HandleEventUserlist( event );
			break;
			}
			
		case GG_EVENT_NOTIFY:
		case GG_EVENT_NOTIFY_DESCR:
			{
			HandleEventNotify( event );
	 		break;
			}
			
		case GG_EVENT_STATUS:
			{
			HandleEventStatus( event );
			break;
			}
			
		case GG_EVENT_NOTIFY60:
			{
			HandleEventNotify60( event );
	 		break;
			}
			
		case GG_EVENT_STATUS60:
			{
			HandleEventStatus60( event );
			break;
			}
		}
	gg_event_free( event );
	}

void NetworkHandler::HandleEventConnected( struct gg_event *event ) {
	fprintf( stderr, "NetworkHandler::HandleEventConnected()\n" );
	fprintf( stderr, "NetworkHandler=> Checking userlist... " );
	if( iNetwork->iWindow->GetProfile()->GetUserlist()->IsValid() == false ||
		iNetwork->iWindow->GetProfile()->GetNeedImport() == true )
		{
		fprintf( stderr, "need import.\n");
		iNetwork->iWindow->GetProfile()->GetUserlist()->Import( iNetwork->Session(), iNetwork->iWindow->GetProfile()->GetUserlist()->GetList() );
		iNetwork->iWindow->GetProfile()->SetNeedImport( false );
		}
	else
		{
		fprintf( stderr, "Ok.\n");
		iNetwork->iWindow->GetProfile()->GetUserlist()->Send( iNetwork->Session() );
		fprintf( stderr, "NetworkHandler: Sent userlist to get notifies from server\n" );
		}
	BMessenger( iNetwork->iWindow ).SendMessage( UPDATE_STATUS );
	BMessage* message = new BMessage( BGDESKBAR_CHSTATE );
	message = new BMessage( BGDESKBAR_CHSTATE );
	message->AddInt16( "iStatus", iNetwork->iStatus );
	BMessenger( iNetwork->iWindow ).SendMessage( message );
	delete message;
}

void NetworkHandler::HandleEventConnFailed( struct gg_event *event )
	{
	fprintf( stderr, "NetworkHandler::HandleEventConnFailed()\n" );
	iNetwork->Logout();
	BMessenger( iNetwork->iWindow ).SendMessage( SET_NOT_AVAIL );
	}

void NetworkHandler::HandleEventDisconnected( struct gg_event *event )
	{
	printf( "NetworkHandler::HandleEventDisconnected()\n" );
	BMessenger( iNetwork->iWindow ).SendMessage( SET_NOT_AVAIL );
 	}

void NetworkHandler::HandleEventMsg( struct gg_event *event )
	{
	fprintf( stderr, "NetworkHandler::HandleEventMsg()\n");
	BMessage *wiadomosc = new BMessage( GOT_MESSAGE );
	wiadomosc->AddInt32( "who", iNetwork->iEvent->event.msg.sender );
	wiadomosc->AddString( "msg", ( char* ) iNetwork->iEvent->event.msg.message );
//	fprintf( stderr, "Od: %d\nWiadomosc: %s\n", iNetwork->iEvent->event.msg.sender, (char*)iNetwork->iEvent->event.msg.message );
	BMessenger( iNetwork ).SendMessage( wiadomosc );
	}

void NetworkHandler::HandleEventUserlist( struct gg_event *event )
	{
	fprintf( stderr, "NetworkHandler::HandleEventUserlist()\n");
	if( iNetwork->iEvent->event.userlist.type == GG_USERLIST_GET_REPLY )
		{
		if( iNetwork->iEvent->event.userlist.reply )
			{
			Userlist *userlist = iNetwork->iWindow->GetProfile()->GetUserlist();
			List *list = userlist->GetList();
			int n;
			n = list->CountItems();
			if( n >= 0 )
				{
				Person *o = NULL;
				for( int i = 0; i < n; i++ )
					{
					o = ( Person* ) list->ItemAt( i );
					if( iNetwork->Session() )
						gg_remove_notify( iNetwork->Session(), o->GetUIN() );
					}
				}
			GaduListItem *g = NULL;
			n = iNetwork->iWindow->ListItems()->CountItems();
			if( iNetwork->iWindow->Lock() ) {
				if( iNetwork->iWindow->ListView()->LockLooper() ) {
					for( int i = 0; i < n; i++ ) {
						g = ( GaduListItem* ) iNetwork->iWindow->ListItems()->ItemAt( i );
						iNetwork->iWindow->ListView()->RemoveItem( g );
					// XXX segfault! g points to wrong data(?)
					//	delete g;
					}
					iNetwork->iWindow->ListView()->UnlockLooper();
				}
				iNetwork->iWindow->ListItems()->MakeEmpty();
				iNetwork->iWindow->Unlock();
			}
			userlist->Set( iNetwork->iEvent->event.userlist.reply );
			fprintf( stderr, "sending userlist...\n" );
			userlist->Send( iNetwork->Session() );
			fprintf( stderr, "sent\n" );
			BMessenger( iNetwork->iWindow ).SendMessage( UPDATE_LIST );
			BAlert* alert = new BAlert( "Lista",
										"Lista kontaktow zostala zaladowana z serwera",
										"OK" );
			alert->Go();
			}
		}
	}

void NetworkHandler::HandleEventNotify( struct gg_event *event )
	{
	fprintf( stderr, "NetworkHandler::HandleEventNotify()\n");
 	struct gg_notify_reply *n;
 	if( iNetwork->iEvent->type == GG_EVENT_NOTIFY )
 		n = iNetwork->iEvent->event.notify;
 	else
 		n = iNetwork->iEvent->event.notify_descr.notify;
	
	for( ; n->uin; n++ )
		{
		Userlist* userlist = iNetwork->iWindow->GetProfile()->GetUserlist();
		List* list = userlist->GetList();
 		Person *o;
 		if( !( o = userlist->Find( n->uin ) ) )
 			{
 			break;
 			}
 		for( int i = 0; i < list->CountItems(); i++ )
 			{
 			o = ( Person* ) list->ItemAt( i );
 			if( o->GetUIN() == n->uin )
 				break;
 			}
 		o->SetStatus( n->status );
		}
//	if( iNetwork->iWindow->ListView()->LockLooper() ) {
//		iNetwork->iWindow->ListView()->MakeEmpty();		// XXX memleak?
//		iNetwork->iWindow->ListView()->UnlockLooper();
//	}
	BMessenger( iNetwork->iWindow ).SendMessage( UPDATE_LIST );
	}

void NetworkHandler::HandleEventNotify60( struct gg_event *event )
	{
	fprintf( stderr,"NetworkHandler::HandleEventNotify60()\n");
	Userlist* userlist = iNetwork->iWindow->GetProfile()->GetUserlist();
	List* list = userlist->GetList();
	for( int i = 0; iNetwork->iEvent->event.notify60[ i ].uin; i++ )
		{
 		Person *o = NULL;
 		if( !( o = userlist->Find( iNetwork->iEvent->event.notify60[ i ].uin ) ) )
 			break;
 		for( int a = 0; a < list->CountItems(); a++ )
 			{
 			o = ( Person* ) list->ItemAt( a );
 			if( o->GetUIN() == iNetwork->iEvent->event.notify60[ i ].uin )
 				break;
 			}
 		o->SetStatus( iNetwork->iEvent->event.notify60[ i ].status );
 		o->SetDescription( iNetwork->iEvent->event.notify60[ i ].descr );
		}
	if( iNetwork->iWindow->ListView()->LockLooper() )
		{
		iNetwork->iWindow->ListView()->MakeEmpty();
		iNetwork->iWindow->ListView()->UnlockLooper();
		}
	BMessenger( iNetwork->iWindow ).SendMessage( UPDATE_LIST );
	}

void NetworkHandler::HandleEventStatus( struct gg_event *event ) {
	fprintf( stderr, "NetworkHandler::HandleEventStatus()\n");
	Userlist* userlist = iNetwork->iWindow->GetProfile()->GetUserlist();
 	Person *o = NULL;
 	if( !( o = userlist->Find( iNetwork->iEvent->event.status.uin ) ) ) {
		fprintf( stderr, "no such person %i\n",iNetwork->iEvent->event.status60.uin);
 		return;
 	}
 	o->SetStatus( iNetwork->iEvent->event.status.status );
 	o->SetDescription( iNetwork->iEvent->event.status.descr );
//	if( iNetwork->iWindow->ListView()->LockLooper()) {
//		iNetwork->iWindow->ListView()->MakeEmpty();	// XXX memleak?
//		iNetwork->iWindow->ListView()->UnlockLooper();
//	}
	BMessenger( iNetwork->iWindow ).SendMessage( UPDATE_LIST );
}

void NetworkHandler::HandleEventStatus60( struct gg_event *event ) {
	fprintf( stderr, "NetworkHandler::HandleEventStatus60()\n" );
	Userlist* userlist = iNetwork->iWindow->GetProfile()->GetUserlist();
	Person* o = NULL;
  	if( !( o = userlist->Find( iNetwork->iEvent->event.status60.uin ) ) ) {
		fprintf( stderr, "no such person %i\n",iNetwork->iEvent->event.status60.uin);
 		return;
 	}
 	o->SetStatus( iNetwork->iEvent->event.status60.status );
 	o->SetDescription( iNetwork->iEvent->event.status60.descr );
//	if( iNetwork->iWindow->ListView()->LockLooper() ) {
//		// XXX memleak? - a usunięcie obiektów?
//		// XXX i w ogóle po co usuwanie tutaj?
//		iNetwork->iWindow->ListView()->MakeEmpty();
//		iNetwork->iWindow->ListView()->UnlockLooper();
//	}
	BMessenger( iNetwork->iWindow ).SendMessage( UPDATE_LIST );
}

static int Expired( time_t timer )
	{
//	fprintf( stderr, "timer = %d curTime = %d\n", timer, curTime );
	if( timer && curTime >= timer )
		return 1;
	return 0;
	}

static void Rearm( time_t* timer, int seconds )
	{
	time( timer );
	*timer += seconds;
	}

void NetworkHandler::HandlePingTimeoutCallback( time_t &pingTimer )
	{
	fprintf( stderr, "NetworkHandler::HandlePingTimeoutCallback( %ld )\n", pingTimer );
	gg_ping( iNetwork->Session() );
	Rearm( &pingTimer, 60 );
	}

Network* NetworkHandler::GetNetwork() const
	{
	fprintf( stderr, "NetworkHandler::GetNetwork()\n" );
	return iNetwork;
	}
