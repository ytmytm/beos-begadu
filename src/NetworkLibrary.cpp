/*
	SiecLib.cpp
	Ten plik jest częscią kodu źródłowego BeGadu.
	Homepage: http://gadu.beos.pl
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

#include <Alert.h>
#include <Message.h>
#include <String.h>

#include "Msg.h"
#include "Network.h"
#include "Main.h"
#include "Chat.h"
#include "NetworkHandler.h"
#include "Person.h"

extern "C"
{
#include "libgadu.h"
}

void Network::GotMsg( uin_t aWho, const char* aMessage )
	{
	fprintf( stderr, "Network::GotMsg()\n" );
	/* sprawdzamy czy mamy aktualnie otwarte okno dla tej osoby :) */
	ChatWindow* win = NULL;
	for( int i = 0; i < iWinList->CountItems(); i++ )
		{
		win = ( ChatWindow* ) iWinList->ItemAt( i );
		if( win->iWho == aWho )
			break;
		}
	if( win && ( win->iWho == aWho ) )
		{
//		win->Activate();
		}
	else
		{
		win = new ChatWindow( this, iWindow, aWho );
		iWinList->AddItem( win );
		win->Show();
		}
	/* i pokazujemy je :P */
	BMessage* wiadomosc = new BMessage( SHOW_MESSAGE );
	wiadomosc->AddString( "msg", aMessage );
	BMessenger( win ).SendMessage( wiadomosc );
	delete wiadomosc;
	}

void Network::AddHandler( int fd, int cond, void* data )
	{
	fprintf( stderr, "Network::AddHandler( fd=%d, cond=%d )\n", fd, cond );
	NetworkHandler* handler = new NetworkHandler( this, iId, fd, cond, data );
	iHandlerList->AddItem( handler );
	handler->Run();
	}

void Network::RemoveHandler( int fd )
	{
	fprintf( stderr, "Network::RemoveHandler( fd=%d )\n", fd );
	NetworkHandler* handler = NULL;
	for( int i = 0; i < iHandlerList->CountItems(); i++ )
		{
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

void Network::SetStatus( int aStatus )
	{
	iStatus = aStatus;
	}

struct gg_session* Network::Session() const
	{
	return iSession;
	}
