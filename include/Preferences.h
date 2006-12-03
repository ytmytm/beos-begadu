/*
 * ============================================================================
 *  Nazwa    : Opcje z Opcje.h
 *  Projekt  : BeGadu
 *  Authorzy : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Opis:
 *		Klasa okna z opcjami
 *  Version  : 1.2
 * ============================================================================
 */

#ifndef __BEGADU_PREFERENCES_H__
#define __BEGADU_PREFERENCES_H__

#include <Window.h>
#include <Resources.h>

/* zewnetrzne klasy, includowane w zrodle */
class Profile;
class BTextControl;
class BCheckBox;
class MainWindow;
class Network;
class Person;
class BBitmap;
class BitmapView;

class Preferences : public BWindow
	{
	public:
		Preferences( Profile* aProfile, MainWindow* aWindow, BRect aRect, BResources* aRes );
		virtual void MessageReceived( BMessage* aMessage );
		virtual void Show();
		BBitmap* GetBitmap( const char* aName );
		
		Profile				*	iProfile;
		MainWindow			*	iWindow;
		BBitmap				*	iLogo;
		BitmapView			*	iLogoView;
		BTextControl		*	iNumberControl;
		BTextControl		*	iPasswordControl;
		BResources			*	iResources;
	};

#endif /* __BEGADU_PREFERENCES_H__ */
