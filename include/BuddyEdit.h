#ifndef __BEGADU_BUDDY_EDIT_H__
#define __BEGADU_BUDDY_EDIT_H__

#include <Window.h>

class BButton;
class MainWindow;
class BTextControl;
class BView;
class Person;

class BuddyEdit : public BWindow
	{
	public:
		BuddyEdit( MainWindow* aWindow, BRect aRect, Person* aPerson );
		virtual void MessageReceived( BMessage* aMessage );
		virtual void Show();

	private:
		Person				*	iPerson;
		MainWindow			*	iWindow;
		BTextControl		*	iNameControl;
		BTextControl		*	iSurnameControl;
		BTextControl		*	iNickControl;
		BTextControl		*	iDisplayControl;
		BTextControl		*	iMobileControl;
		BTextControl		*	iEmailControl;
	};

#endif /* __BEGADU_BUDDY_EDIT_H__ */
