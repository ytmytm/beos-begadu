/*
	Siec.cpp
	Ten plik jest częscią kodu źródłowego BeGadu.
	Homepage: http://gadu.beos.pl
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <File.h>
#include <Path.h>
#include <FindDirectory.h>
#include <Message.h>
#include <String.h>
#include "Msg.h"
#include "Network.h"
#include "Main.h"
#include "Chat.h"
#include "Person.h"
#include "NetworkHandler.h"
extern "C"
{
#include "libgadu.h"
}

Network::Network( Profile* aProfile, List* aList ) : BLooper( "Network Loop" )
	{
	fprintf( stderr, "Network::Network()\n" );
	/* inicjalizacja */
	iProfile		= aProfile;
	iList			= aList;
	iWindow			= NULL;
	iHandlerList	= new BList( 256 );
	iWinList		= new BList( 512 );
	iIdent			= 0;
	iSession		= NULL;
	iStatus			= GG_STATUS_NOT_AVAIL;
	
	/* czekamy na wiadomości :)) */
	Run();
	}

void Network::Quit()
	{
	fprintf( stderr, "Network::Quit()\n" );
	/* Rozłączamy się */
	if( ( iStatus != GG_STATUS_NOT_AVAIL ) || ( iStatus != GG_STATUS_NOT_AVAIL_DESCR ) )
		Logout();
	Lock();
	BLooper::Quit();
	}

void Network::MessageReceived( BMessage* aMessage )
	{
	switch( aMessage->what )
		{
		/*
			wiadomości otrzymane od libgadu callback
			cała obsługa jest w SiecLib.cpp, my tylko
			wywołujemy je stąd :)
		*/
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

		/*
			wiadomości otrzymane od interfejsu
			cała obsługa jest w SiecInt.cpp, my tylko
			wywołujemy je stąd :)
		*/
		
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
			
		case ADD_PERSON:
			{
			fprintf( stderr, "Network::MessageReceived( ADD_PERSON )\n" );
			// do zaimplementowania
			break;
			}
			
		case DEL_PERSON:
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
			if( ( win = GetMesgWinForUser( who ) ) )
				{
				if( win->LockLooper() )
					{
					win->Activate();
					win->UnlockLooper();
					}
				}
			else // a jeśli nie, tworzymy nowe :)
				{
				win = new ChatWindow( this, iWindow, who );
				iWinList->AddItem( win );
				Person* person;
				if( ( person = GetPersonForUser( who ) ) )
					{
					BMessage *newmessage = new BMessage( UPDATE_STATUS );
					newmessage->AddInt32( "status", person->iStatus );
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
			}
			
		default:
			BLooper::MessageReceived( aMessage );
		}
	}

void Network::GotWindow( MainWindow* aWindow )
	{
	fprintf( stderr, "Network::GotWindow( %p )\n", aWindow );
	iWindow = aWindow;
//	if( ( iWindow = aWindow ) ) // or =
//		BMessenger( iWindow ).SendMessage( UPDATE_STATUS );
	}

/* zwracamy wskaznik do okna jesli rozmowcy jesli z nim juz rozmawiamy */
ChatWindow* Network::GetMesgWinForUser( uin_t aWho )
	{
	fprintf( stderr, "Network::GotMesgWinForUser( %d )\n", aWho );
	ChatWindow *win = NULL;
	for( int i = 0; i < iWinList->CountItems(); i++ )
		{
		win = ( ChatWindow* ) iWinList->ItemAt( i );
		if( win->iWho == aWho )
			break;
		}
	
	if( win && (win->iWho == aWho ) )
		return win;
	
	return NULL;
	}

/* zwracamy wskaznik do osoby jesli z taka rozmawiamy */
Person* Network::GetPersonForUser( uin_t aWho )
	{
	fprintf( stderr, "Network::GotPersonForUser( %d )\n", aWho );
	Person* person = NULL;
	for( int i = 0; i < iWindow->GetProfile()->GetUserlist()->GetList()->CountItems(); i++ )
		{
		person = ( Person* ) iWindow->GetProfile()->GetUserlist()->GetList()->ItemAt( i );
		if( person->iUIN == aWho )
			break;
		}
	if( person && ( person->iUIN == aWho ) )
		return person;

	return NULL;
	}
