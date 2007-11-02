
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
	fprintf( stderr, "Network::Network()\n" );
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
	fprintf( stderr, "Network::Quit()\n" );
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
			fprintf( stderr, "Network::MessageReceived( ADD_HANDLER )\n" );
			int fd, cond;
			void *data;
			aMessage->FindInt32( "fd", ( int32* ) &fd );
			aMessage->FindInt32( "cond", ( int32* ) &cond );
			aMessage->FindPointer( "data", &data );
			AddHandler( fd, cond, data );
			break;
			}
		case DEL_HANDLER:
			{
			fprintf( stderr, "Network::MessageReceived( DEL_HANDLER )\n" );
			int fd;
			aMessage->FindInt32( "fd", ( int32* ) &fd );
			RemoveHandler( fd );
			break;
			}
		case GOT_MESSAGE:
			{
			fprintf( stderr, "Network::MessageReceived( GOT_MESSAGE )\n" );
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
			fprintf( stderr, "Network::MessageReceived( DO_LOGIN )\n" );
			Login();
			break;
			}
		case DO_LOGOUT:
			{
			fprintf( stderr, "Network::MessageReceived( DO_LOGOUT )\n" );
			Logout();
			break;
			}
		case ADD_BUDDY:
			{
			fprintf( stderr, "Network::MessageReceived( ADD_PERSON )\n" );
			// do zaimplementowania
			break;
			}
		case DEL_BUDDY:
			{
			fprintf( stderr, "Network::MessageReceived( DEL_PERSON )\n" );
			// do zaimplementowania
			break;
			}
		case OPEN_MESSAGE:
			{
			fprintf( stderr, "Network::MessageReceived( OPEN_MESSAGE )\n" );
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
			fprintf( stderr, "Network::MessageReceived( SEND_MESSAGE )\n" );
			int 			who;
			const char *	msg;
			aMessage->FindInt32( "who", ( int32* ) &who );
			aMessage->FindString( "msg", &msg );
			SendMsg( who, msg );
			break;
			}
		case CLOSE_MESSAGE:
			{
			fprintf( stderr, "Network::MessageReceived( CLOSE_MESSAGE )\n" );
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
		if( win->iWho == aWho )
			break;
	}

	if( win && (win->iWho == aWho ) )
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
	printf( "Network::Login()\n" );
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
	printf( "Network::Login(status=%i)\n", status );
	Login(status, new BString(""));
}

void Network::Login( int aStatus, BString *aDescription ) {
	printf( "Network::Login(status=%i, description=[%s])\n",aStatus,aDescription->String() );
	/* ustawiamy status na "Łączenie" */
	SetStatus(aStatus);
	SetDescription(aDescription);
	/* ustawiamy pola potrzebne do połączenia z gg */
	memset( &iLoginParam, 0, sizeof( iLoginParam ) );
	iLoginParam.uin = iProfile->GetUIN();
	iLoginParam.password = ( char* ) iProfile->GetPassword();
//	iLoginParam.async = 1;
	iLoginParam.async = 0;
	iLoginParam.status = iStatus;
	iLoginParam.status_descr = ( char* ) iDescription->String();
//	gg_debug_level = ~0;
	gg_debug_level = 255;
	BMessenger( this ).SendMessage( ADD_HANDLER );
	if( iWindow )
	 	BMessenger( iWindow ).SendMessage( UPDATE_STATUS );
}

void Network::Logout() {
	printf( "Network::Logout()\n" );
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
	printf( "Network::SendMsg()\n" );
	if( iSession ) {
		BString *msg = toISO2(aMessage);
		if( gg_send_message( iSession, GG_CLASS_CHAT, aWho, ( unsigned char* ) msg->String() ) == -1 ) {
			gg_free_session( iSession );
			perror( "Connection lost." );
		}
		delete msg;
//		else
//			fprintf(stderr,"Wysłałem wiadomość o treści %s do %d\n", komu, wiadomosc);
	}
}

void Network::GotMsg( uin_t aWho, const char* aMessage ) {
	printf( "Network::GotMsg()\n" );
	/* sprawdzamy czy mamy aktualnie otwarte okno dla tej osoby :) */
	ChatWindow* win = NULL;
	for( int i = 0; i < iWinList->CountItems(); i++ ) {
		win = ( ChatWindow* ) iWinList->ItemAt( i );
		if( win->iWho == aWho )
			break;
	}

	if( win && ( win->iWho == aWho ) ) {
//		win->Activate();
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

void Network::AddHandler( int fd, int cond, void* data ) {
	printf( "Network::AddHandler()\n" );
	NetworkHandler* handler;
	handler = new NetworkHandler( this, fd, cond, data );
	iHandlerList->AddItem( handler );
	handler->Run();
}

void Network::RemoveHandler( int fd ) {
	printf( "Network::RemoveHandler()\n" );
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

struct gg_session* Network::Session() const {
	return iSession;
}
