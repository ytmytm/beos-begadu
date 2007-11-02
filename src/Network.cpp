
#include <Message.h>
#include <String.h>
#include <ListView.h>
// for toISO/fromISO
#include <GfxStuff.h>

#include "Msg.h"
#include "Network.h"
#include "Main.h"
#include "Chat.h"
#include "Person.h"
#include "NetworkHandler.h"
#include "globals.h"

Network::Network( Profile* aProfile ) : BLooper( "Network Loop" ) {
	DEBUG_TRACE( "Network::Network()\n" );
	/* inicjalizacja */
	iProfile		= aProfile;
	iWindow			= NULL;
	iHandlerList	= new BList( 256 );
	iWinList		= new BList( 512 );
	iSession		= NULL;
	iStatus			= GG_STATUS_NOT_AVAIL;
	iDescription	= new BString( "" );

	/* czekamy na wiadomości :)) */
	Run();
}

void Network::Quit() {
	DEBUG_TRACE( "Network::Quit()\n" );
	/* Rozłączamy się */
	if( ( iStatus != GG_STATUS_NOT_AVAIL ) && ( iStatus != GG_STATUS_NOT_AVAIL_DESCR ) )
		Logout();
	ChatWindow* chat;
	for( int i = 0; i < iWinList->CountItems(); i++ ) {
		chat = ( ChatWindow* ) iWinList->ItemAt( i );
		if( chat->Lock() )
			chat->Quit();
	}
	iWinList->MakeEmpty();
	Lock();
	BLooper::Quit();
}

void Network::MessageReceived( BMessage* aMessage ) {
	switch( aMessage->what ) {
		// messages from libgadu
		case ADD_HANDLER:
			{
			DEBUG_TRACE( "Network::MessageReceived( ADD_HANDLER )\n" );
			int fd;
			aMessage->FindInt32( "fd", ( int32* ) &fd );
			AddHandler( fd );
			break;
			}
		case DEL_HANDLER:
			{
			DEBUG_TRACE( "Network::MessageReceived( DEL_HANDLER )\n" );
			int fd;
			aMessage->FindInt32( "fd", ( int32* ) &fd );
			RemoveHandler( fd );
			break;
			}
		case GOT_MESSAGE:
			{
			DEBUG_TRACE( "Network::MessageReceived( GOT_MESSAGE )\n" );
			int				who;
			const char *	msg;
			aMessage->FindInt32( "who", ( int32* ) &who );
			aMessage->FindString("msg", &msg );
			GotMsg( who, msg );
			break;
			}
		// messages from interface
		case DO_LOGIN:
			{
			DEBUG_TRACE( "Network::MessageReceived( DO_LOGIN )\n" );
			Login();
			break;
			}
		case DO_LOGOUT:
			{
			DEBUG_TRACE( "Network::MessageReceived( DO_LOGOUT )\n" );
			Logout();
			break;
			}
		case ADD_BUDDY:
			{
			DEBUG_TRACE( "Network::MessageReceived( ADD_BUDDY )\n" );
			// do zaimplementowania
			break;
			}
		case DEL_BUDDY:
			{
			DEBUG_TRACE( "Network::MessageReceived( DEL_BUDDY )\n" );
			// do zaimplementowania
			break;
			}
		case OPEN_MESSAGE:
			{
			DEBUG_TRACE( "Network::MessageReceived( OPEN_MESSAGE )\n" );
			int	who;
			aMessage->FindInt32( "who", ( int32* ) &who );
			ChatWindow *win;
			// jeśli mamy już otwarte okno z tą osobą, przejdźmy do niego
			if( ( win = GetMesgWinForUser( who ) ) ) {
				if( win->LockLooper() ) {
					win->Activate();
					win->UnlockLooper();
				}
			} else { // a jeśli nie, tworzymy nowe :)
				win = new ChatWindow( this, iWindow, who );
				iWinList->AddItem( win );
				Person* person;
				if( ( person = GetPersonForUser( who ) ) ) {
					BMessage *newmessage = new BMessage( UPDATE_STATUS );
					newmessage->AddInt32( "status", person->GetStatus() );
					BMessenger( win ).SendMessage( newmessage );
					delete newmessage;
				}
				win->Show();
			}
			break;
			}
		case SEND_MESSAGE:
			{
			DEBUG_TRACE( "Network::MessageReceived( SEND_MESSAGE )\n" );
			int 			who;
			const char *	msg;
			aMessage->FindInt32( "who", ( int32* ) &who );
			aMessage->FindString( "msg", &msg );
			SendMsg( who, msg );
			break;
			}
		case CLOSE_MESSAGE:
			{
			DEBUG_TRACE( "Network::MessageReceived( CLOSE_MESSAGE )\n" );
			ChatWindow	*win;
			aMessage->FindPointer( "win", ( void** ) &win );
			iWinList->RemoveItem( win );
			if( win->Lock() )
				win->Quit();
			break;
			}
		default:
			BLooper::MessageReceived( aMessage );
			break;
		}
}

void Network::GotWindow( MainWindow* aWindow ) {
	fprintf( stderr, "Network::GotWindow( %p )\n", aWindow );
	iWindow = aWindow;
}

/* zwracamy wskaznik do okna jesli rozmowcy jesli z nim juz rozmawiamy */
ChatWindow* Network::GetMesgWinForUser( uin_t aWho ) {
	fprintf( stderr, "Network::GotMesgWinForUser( %d )\n", aWho );
	ChatWindow *win = NULL;

	for( int i = 0; i < iWinList->CountItems(); i++ ) {
		win = ( ChatWindow* ) iWinList->ItemAt( i );
		if( win->Who() == aWho )
			break;
	}

	if( win && (win->Who() == aWho ) )
		return win;

	return NULL;
}

/* zwracamy wskaznik do osoby jesli z taka rozmawiamy */
Person* Network::GetPersonForUser( uin_t aWho ) {
	fprintf( stderr, "Network::GotPersonForUser( %d )\n", aWho );
	Person* person = NULL;
	for( int i = 0; i < iWindow->GetProfile()->GetUserlist()->GetList()->CountItems(); i++ ) {
		person = ( Person* ) iWindow->GetProfile()->GetUserlist()->GetList()->ItemAt( i );
		if( person->GetUIN() == aWho )
			break;
	}
	if( person && ( person->GetUIN() == aWho ) )
		return person;

	return NULL;
}

void Network::Login() {
	DEBUG_TRACE( "Network::Login()\n" );
	int state = iProfile->GetAutoStatus();
	switch (state) {
		case GG_STATUS_NOT_AVAIL:
		case GG_STATUS_NOT_AVAIL_DESCR:
			break;
		case GG_STATUS_AVAIL_DESCR:
		case GG_STATUS_BUSY_DESCR:
		case GG_STATUS_INVISIBLE_DESCR:
			Login(state,new BString(iProfile->GetDescription()));
			break;
		default:
			Login(state);
			break;
	}
}

void Network::Login( int status ) {
	fprintf(stderr, "Network::Login(status=%i)\n", status );
	Login(status, new BString(""));
}

void Network::Login( int aStatus, BString *aDescription ) {
	fprintf(stderr, "Network::Login(status=%i, description=[%s])\n",aStatus,aDescription->String() );
	/* ustawiamy status na "Łączenie" */
	SetStatus(aStatus);
	SetDescription(aDescription);
	/* ustawiamy pola potrzebne do połączenia z gg */
	memset( &iLoginParam, 0, sizeof( iLoginParam ) );
	iLoginParam.uin = iProfile->GetUIN();
	iLoginParam.password = ( char* ) iProfile->GetPassword();
//	iLoginParam.async = 1;
	iLoginParam.async = 0;
	iLoginParam.status = Status();
	iLoginParam.status_descr = ( char* ) iDescription->String();
//	gg_debug_level = ~0;
	gg_debug_level = 255;
	BMessenger( this ).SendMessage( ADD_HANDLER );
	if( iWindow )
	 	BMessenger( iWindow ).SendMessage( UPDATE_STATUS );
}

void Network::Logout() {
	DEBUG_TRACE( "Network::Logout()\n" );
	/* po prostu sie wylogowujemy */
	if( iSession ) {
		if( iStatus != GG_STATUS_NOT_AVAIL_DESCR )
			iStatus = GG_STATUS_NOT_AVAIL;
		gg_logoff( iSession );
		gg_free_session( iSession );
		iSession = NULL;
		/* zatrzymujemy wszystkie handlery */
		NetworkHandler *handler;
		for( int i = iHandlerList->CountItems(); i > 0; i-- ) {
			handler = ( NetworkHandler* ) iHandlerList->ItemAt( i - 1 );
			RemoveHandler( handler->iFd );
		}
		/* uaktualniamy statusy ludzi z listy */
		if( iWindow ) {
			Person* p = NULL;
			for( int i = 0; i < iWindow->ListItems()->CountItems(); i++ ) {
				p = ( Person* ) iWindow->ListItems()->ItemAt( i );
				p->SetStatus( GG_STATUS_NOT_AVAIL );
			}
			/* uaktualniamy liste */
			// XXX memleak?
			if( iWindow->ListView()->LockLooper() ) {
				iWindow->ListView()->MakeEmpty();
		 		iWindow->ListView()->UnlockLooper();
		 	}
			BMessenger( iWindow ).SendMessage( UPDATE_LIST );
		}
		/* uaktualniamy status */
		if ( iWindow ) {
			BMessage *message = new BMessage(SET_CONN_DESCRIPTION);
			message->AddString("desc",BString(_T("Disconnected")));
			BMessenger( iWindow ).SendMessage( message );
			delete message;

			BMessenger( iWindow ).SendMessage( UPDATE_STATUS );
		}
	}
}

void Network::SendMsg( uin_t aWho, const char* aMessage ) {
	DEBUG_TRACE( "Network::SendMsg()\n" );
	if( iSession ) {
		BString *msg = toISO2(aMessage);
		if( gg_send_message( iSession, GG_CLASS_CHAT, aWho, ( unsigned char* ) msg->String() ) == -1 ) {
			gg_free_session( iSession );
			perror( "Connection lost." );
			// XXX handle event disconnected?
		}
		delete msg;
//		else
//			fprintf(stderr,"Wysłałem wiadomość o treści %s do %d\n", komu, wiadomosc);
	}
}

void Network::GotMsg( uin_t aWho, const char* aMessage ) {
	DEBUG_TRACE( "Network::GotMsg()\n" );
	/* sprawdzamy czy mamy aktualnie otwarte okno dla tej osoby :) */
	ChatWindow* win = NULL;
	for( int i = 0; i < iWinList->CountItems(); i++ ) {
		win = ( ChatWindow* ) iWinList->ItemAt( i );
		if( win->Who() == aWho )
			break;
	}

	if( win && ( win->Who() == aWho ) ) {
		win->Activate();
	} else {
		win = new ChatWindow( this, iWindow, aWho );
		iWinList->AddItem( win );
		win->Show();
	}
	BString *txt = fromISO2(aMessage);
	/* i pokazujemy je :P */
	BMessage* wiadomosc = new BMessage( SHOW_MESSAGE );
	wiadomosc->AddString( "msg", txt->String() );
	BMessenger( win ).SendMessage( wiadomosc );
	delete wiadomosc;
	delete txt;
}

void Network::AddHandler( int fd ) {
	fprintf(stderr, "Network::AddHandler(%i)\n", fd );
	NetworkHandler* handler;
	handler = new NetworkHandler( this, fd );
	iHandlerList->AddItem( handler );
	handler->Run();
}

void Network::RemoveHandler( int fd ) {
	fprintf(stderr, "Network::RemoveHandler(%i)\n", fd );
	NetworkHandler* handler = NULL;
	for( int i = 0; i < iHandlerList->CountItems(); i++ ) {
		handler = ( NetworkHandler* ) iHandlerList->ItemAt( i );
		if( handler->iFd == fd )
			break;
	}
	if( !handler || ( handler->iFd != fd ) )
		return;
	handler->Stop();
	iHandlerList->RemoveItem( handler );
	delete handler;
}

void Network::SetStatus( int aStatus ) {
	iStatus = aStatus;
}

void Network::SetDescription( BString *aDescription ) {
	if (iDescription)
		delete iDescription;
	iDescription = aDescription;
}

void Network::SetSession(struct gg_session *s ) {
	iSession = s;
}
