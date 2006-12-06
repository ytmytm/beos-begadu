/*
 * ============================================================================
 *  Nazwa    : Description from Description.h
 *  Projekt  : BeGadu
 *  Authorzy : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Opis:
 *		Klasa okna z opcjami
 *  Version  : 1.2
 * ============================================================================
 */

#ifndef __BEGADU_DESCRIPTION_H__
#define __BEGADU_DESCRIPTION_H__

#include <Window.h>
#include <Resources.h>

class BTextView;
class BRadioButton;
class BButton;
class MainWindow;
class BBitmap;
class BBox;
class BView;

class IconsView : public BView
	{
	public:
		IconsView( BRect aRect, BResources *aResources );
		~IconsView();
		void AttachedToWindow();
		void Draw( BRect aRect );

	private:
		BBitmap* GetBitmap( const char* aName );

		BResources 			*	iResources;
		BBitmap				*	iIconAvail;
		BBitmap				*	iIconBusy;
		BBitmap				*	iIconInvisible;
		BBitmap				*	iIconNotAvail;
	};

class Description : public BWindow
	{
	public:
		Description( MainWindow* aWindow, BRect aRect, BResources* aRes );
		virtual void MessageReceived( BMessage* aMessage );
		virtual void Show();

	private:		
		MainWindow			*	iWindow;
		
		/* field to enter description */
		BTextView			*	iDescription;
		IconsView			*	iIconsView;
		
		/* radio buttons */
		BRadioButton		*	iAvail;
		BRadioButton		*	iBusy;
		BRadioButton		*	iInvisible;
		BRadioButton		*	iNotAvail;
		BBox				*	iBox;
	
		/* resources */
		BResources			*	iResources;
		int						iStatus;
	};

#endif /* __BEGADU_DESCRIPTION_H__ */
