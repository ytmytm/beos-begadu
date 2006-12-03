/*
	SiecInt.cpp
	Ten plik jest częscią kodu źródłowego BeGadu.
	Homepage: http://gadu.beos.pl
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <Message.h>
#include <errno.h>
#include <String.h>
#include <OutlineListView.h>

#include "Msg.h"
#include "Network.h"
#include "Main.h"
#include "Person.h"
#include "NetworkHandler.h"

extern "C"
{
#include "libgadu.h"
}

void Network::Login()
	{
	fprintf( stderr, "Network::Login()\n" );
	/* ustawiamy status na "Łączenie" */
	iStatus = BEGG_CONNECTING;
//	if( iWindow )
//		BMessenger( iWindow ).SendMessage( UPDATE_STATUS );
	/* ustawiamy pola potrzebne do połączenia z gg */
	memset( &iLoginParam, 0, sizeof( iLoginParam ) );
	iLoginParam.uin = iProfile->iNumber;
	iLoginParam.password = ( char* ) iProfile->iPassword->String();
//	iLoginParam.async = 1;
	iLoginParam.async = 0;
	iLoginParam.status = iProfile->AutoStatus();
//	gg_debug_level = ~0;
	BMessenger( this ).SendMessage( ADD_HANDLER );
	if( iWindow )
		BMessenger( iWindow ).SendMessage( UPDATE_STATUS );
	}

void Network::Login( int status )
	{
	fprintf( stderr, "Network::Login(%d)\n", status );
	/* ustawiamy status na "Łączenie" */
	iStatus = status;
//	if( iWindow )
//		BMessenger( iWindow ).SendMessage( UPDATE_STATUS );
	/* ustawiamy pola potrzebne do połączenia z gg */
	memset( &iLoginParam, 0, sizeof( iLoginParam ) );
	iLoginParam.uin = iProfile->iNumber;
	iLoginParam.password = ( char* ) iProfile->iPassword->String();
	iLoginParam.async = 0;
//	iLoginParam.async = 1;
	iLoginParam.status = iStatus;
//	gg_debug_level = ~0;
	BMessenger( this ).SendMessage( ADD_HANDLER );
	if( iWindow )
	 	BMessenger( iWindow ).SendMessage( UPDATE_STATUS );
	}

void Network::Logout()
	{
	fprintf( stderr, "Network::Logout()\n" );
	/* poprostu sie wylogowujemy */
	if( iSession )
		{
		iStatus = GG_STATUS_NOT_AVAIL;
		gg_logoff( iSession );
		gg_free_session( iSession );
		iSession = NULL;
		/* zatrzymujemy wszystkie handlery */
		NetworkHandler *handler;
		for( int i = iHandlerList->CountItems(); i > 0; i-- )
			{
			handler = ( NetworkHandler* ) iHandlerList->ItemAt( i - 1 );
			RemoveHandler( handler->iFd );
			}
		/* uaktualniamy statusy ludzi z listy */
		if( iWindow )
			{
			Person* p = NULL;
			for( int i = 0; i < iWindow->ListItems()->CountItems(); i++ )
				{
				p = ( Person* ) iWindow->ListItems()->ItemAt( i );
				p->iStatus = GG_STATUS_NOT_AVAIL;
			}
		
			/* uaktualniamy liste */
			if( iWindow->ListView()->LockLooper() )
				{
				iWindow->ListView()->MakeEmpty();
		 		iWindow->ListView()->UnlockLooper();
		 		}
			BMessenger( iWindow ).SendMessage( UPDATE_LIST );
			}
		
		/* uaktualniamy status */
		if( iWindow )
			BMessenger( iWindow ).SendMessage( UPDATE_STATUS );
		}
	}

/* wysyłamy wiadomość */
void Network::SendMsg( uin_t aWho, const char* aMessage )
	{
	fprintf( stderr, "Network::SendMsg()\n" );
	if( iSession )
		{
		if( gg_send_message( iSession, GG_CLASS_CHAT, aWho, ( unsigned char* ) aMessage ) == -1 )
			{	
			gg_free_session( iSession );
			perror( "Connection lost." );
			}
//		else
//			fprintf(stderr,"Wysłałem wiadomość o treści %s do %d\n", komu, wiadomosc);
		}
	}
