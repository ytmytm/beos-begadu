
#ifndef __BEGADU_CHATWINDOW_H__
#define __BEGADU_CHATWINDOW_H__

#include <Window.h>
#include <libgadu.h>

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
		uin_t const Who() { return iWho; }

	private:
		Network			*	iNetwork;
		MainWindow		*	iWindow;
		uin_t				iWho;
		BTextControl	*	iSayControl;
		BScrollView		*	iScrollView;
		BTextView		*	iChat;
	};

#endif // __BEGADU_CHATWINDOW_H__
