/*
	Opcje.cpp
	Ten plik jest częscią kodu źródłowego BeGadu.
	Homepage: http://begadu.sf.net
*/

#include <stdio.h>
#include <stdlib.h>
#include <View.h>
#include <String.h>
#include <TextControl.h>
#include <Button.h>
#include <Bitmap.h>
#include <Screen.h>
#include <TabView.h>
#include <CheckBox.h>
#include <Box.h>
#include <OutlineListView.h>
#include <ListView.h>
#include <ListItem.h>

#include "Msg.h"
#include "Preferences.h"
#include "Main.h"
#include "GfxStuff.h"
#include "Network.h"
#include "Person.h"

#define PREFERENCES_NAME "Ustawienia"

Preferences::Preferences( Profile* aProfile, MainWindow* aWindow, BRect aRect, BResources* aRes ) : BWindow( aRect, PREFERENCES_NAME, B_FLOATING_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_NOT_MOVABLE | B_ASYNCHRONOUS_CONTROLS )
	{
	iProfile = aProfile;
	iWindow = aWindow;
	iResources = aRes;
	BRect r = Bounds();

	iLogoView = new BitmapView( BRect( r.left , r.top,
									   r.left + 600, r.top + 150 ),
									   "logo", aRes );
	AddChild( iLogoView );
	
	r.left = 20;
	r.top = 100;
	r.right = r.left + 250;
	r.bottom = r.top + 25;
	iNumberControl = new BTextControl( r, "iNumberControl", "Numer GG:", "0", NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE );
	AddChild( iNumberControl );
	r.left = 20;
	r.right = r.left + 250;
	r.top = 130;
	r.bottom = r.top + 25;
	iPasswordControl = new BTextControl( r, "iPasswordControl", "Haslo:", "", NULL, B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_NAVIGABLE );
	AddChild( iPasswordControl );
	r = Bounds();
	BButton *przycisk;
    przycisk = new BButton( BRect( r.left + 360, r.bottom - 30,
    							   r.left + 440, r.bottom - 5),
    							   "przycisk ok", "Ok",
    							   new BMessage( PREFERENCES_OK ) );
    przycisk->MakeDefault(true);
    AddChild(przycisk);
    
    przycisk = new BButton( BRect( r.left + 270, r.bottom - 30,
    							   r.left + 350, r.bottom - 5),
    							   "przycisk anuluj", "Anuluj",
    							   new BMessage( PREFERENCES_CANCEL ) );
    AddChild(przycisk);

	/* pobieramy aktualna konfiguracje */

    if( iNumberControl->LockLooper() )
    	{
		BString a;
		a << ( int32 ) iProfile->iNumber;
        iNumberControl->SetText( a.String() );
        iPasswordControl->SetText( iProfile->iPassword->String() );
		fprintf( stderr, "numer: %s\nhaslo: %s\n", a.String(), iProfile->iPassword->String() );
        iNumberControl->UnlockLooper();
    	}    
	}

void Preferences::MessageReceived( BMessage* aMessage )
	{
	switch( aMessage->what )
		{
		/* anuluj zmiany */
		case PREFERENCES_CANCEL:
			{
			BMessenger( this ).SendMessage( B_QUIT_REQUESTED );
			break;
			}
		
		case PREFERENCES_OK:
			{
			if( iNumberControl->LockLooper())
				{
				iProfile->iNumber = atoi( iNumberControl->Text() );
				iNumberControl->UnlockLooper();
				}
			if( iPasswordControl->LockLooper())
				{
				iProfile->iPassword->SetTo( iPasswordControl->Text() );
				iPasswordControl->UnlockLooper();
				}
			BMessenger( this ).SendMessage( B_QUIT_REQUESTED );
			break;
			}
			
		default:
			BWindow::MessageReceived( aMessage );
		}
	}

void Preferences::Show()
	{
    BWindow::Show();
	}

BBitmap* Preferences::GetBitmap( const char* aName )
	{
	BBitmap 	*bitmap = NULL;
	size_t 		len = 0;
	status_t 	error;	

	const void *data = iResources->LoadResource( 'BBMP', aName, &len );

	BMemoryIO stream(data, len);
	
	BMessage archive;
	error = archive.Unflatten(&stream);
	if (error != B_OK)
		return NULL;
	bitmap = new BBitmap(&archive);
	if(!bitmap)
		return NULL;

	if(bitmap->InitCheck() != B_OK)
	{
		delete bitmap;
		return NULL;
	}
	
	return bitmap;
}
