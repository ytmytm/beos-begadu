/*
 * ============================================================================
 *  Nazwa    : Chat z Chat.h
 *  Projekt  : BeGadu
 *  Authorzy : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Opis:
 *		Klasa okienka z czatem
 *  Version  : 1.2
 * ============================================================================
 */

#ifndef __BEGADU_CHATWINDOW_H__
#define __BEGADU_CHATWINDOW_H__

#include <Window.h>
#include <libgadu.h>

/* Forward references */
class Network;
class MainWindow;
class Person;
class BTextControl;
class BScrollView;
class BTextView;

class ChatWindow : public BWindow
	{
	public:
		ChatWindow( Network *aNetwork, MainWindow *aWindow, uin_t aWho );
		void MessageReceived( BMessage *aMessage );
		bool QuitRequested();
		void FrameResized( float, float );
		void Show();
		void WindowActivated( bool aActivated );
		
		Network			*	iNetwork;
		MainWindow		*	iWindow;
		uin_t				iWho;
		BTextControl	*	iSayControl;
		BScrollView		*	iScrollView;
		BTextView		*	iChat;
	};

#endif // __BEGADU_CHATWINDOW_H__
