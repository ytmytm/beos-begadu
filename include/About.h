/*
 * ============================================================================
 *  Nazwa    : AboutWindow z About.h
 *  Projekt  : BeGadu
 *  Authorzy : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Opis:
 *		Klasa okienka About
 *  Version  : 1.2
 * ============================================================================
 */

#ifndef __BEGADU_ABOUT_WINDOW_H__
#define __BEGADU_ABOUT_WINDOW_H__

#include <Window.h>
#include <View.h>

class AboutView : public BView
	{
	public:
		AboutView( BRect aRect );
		virtual void Draw( BRect aRect );
	};

class AboutWindow : public BWindow
	{
	public:
		AboutWindow();
		virtual void Show();
		virtual bool QuitRequested();
	};

#endif // __BEGADU_ABOUT_WINDOW_H__
