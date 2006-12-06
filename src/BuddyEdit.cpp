#include <Application.h>
#include <Button.h>
#include <Path.h>
#include <Screen.h>
#include <String.h>
#include <Roster.h>
#include <TextControl.h>
#include <View.h>

#include <stdio.h>
#include <stdlib.h>

#include "Msg.h"
#include "BuddyEdit.h"
#include "Main.h"
#include "Person.h"

#ifdef ZETA
#include <locale/Locale.h>
#else
#define _T(str) (str)
#endif

#define BUDDY_EDIT_NAME "Edycja "

BuddyEdit::BuddyEdit( MainWindow* aWindow, BRect aRect, Person* aPerson )
	: BWindow( aRect,
			   BUDDY_EDIT_NAME,
			   B_TITLED_WINDOW,
			   B_NORMAL_WINDOW_FEEL )
	{

#ifdef ZETA
	app_info appinfo;
	be_app->GetAppInfo( &appinfo );
	BPath localization;
	BEntry entryloc( &appinfo.ref, true );
	entryloc.GetPath( &localization );
	localization.GetParent( &localization );
	localization.Append( "Language/Dictionaries/BeGadu" );
	BString localization_string;
	if( localization.InitCheck() != B_OK )
		localization_string.SetTo( "Language/Dictionaries/BeGadu" );
	else
		localization_string.SetTo( localization.Path() );
	be_locale.LoadLanguageFile( localization_string.String() );
#endif
	
	iPerson = aPerson;

	BString title( BUDDY_EDIT_NAME );
	title.Append( aPerson->GetDisplay() );
	SetTitle( title.String() );

	iWindow = aWindow;
	BFont *font = new BFont( be_plain_font );
	font->SetSize( 12.0 );

	BRect r = Bounds();
	r.left = 5;
	r.right = r.right - 5;
	r.top = 10;
	r.bottom = r.top + 25;

	iNameControl = new BTextControl( r,
									 "iNameControl",
									 _T( "Name:" ),
									"",
									NULL );
	iNameControl->SetDivider( 75 );
	iNameControl->SetAlignment( B_ALIGN_RIGHT, B_ALIGN_LEFT );
	iNameControl->SetFont( font );
	AddChild( iNameControl );

	r.top = r.top + 25;
	r.bottom = r.top + 25;
	
	iSurnameControl = new BTextControl( r,
									 "iSurnameControl",
									 _T( "Surname:" ),
									"",
									NULL );
	iSurnameControl->SetDivider( 75 );
	iSurnameControl->SetAlignment( B_ALIGN_RIGHT, B_ALIGN_LEFT );
	iSurnameControl->SetFont( font );
	AddChild( iSurnameControl );

	r.top = r.top + 25;
	r.bottom = r.top + 25;

	iNickControl = new BTextControl( r,
									 "iNickControl",
									 _T( "Nickname:" ),
									"",
									NULL );
	iNickControl->SetDivider( 75 );
	iNickControl->SetAlignment( B_ALIGN_RIGHT, B_ALIGN_LEFT );
	iNickControl->SetFont( font );
	AddChild( iNickControl );

	r.top = r.top + 25;
	r.bottom = r.top + 25;

	iDisplayControl = new BTextControl( r,
									 "iDisplayControl",
									 _T( "Display as:" ),
									"",
									NULL );
	iDisplayControl->SetDivider( 75 );
	iDisplayControl->SetAlignment( B_ALIGN_RIGHT, B_ALIGN_LEFT );
	iDisplayControl->SetFont( font );
	AddChild( iDisplayControl );

	r.top = r.top + 25;
	r.bottom = r.top + 25;

	iMobileControl = new BTextControl( r,
									 "iMobileControl",
									 _T( "Mobile:" ),
									"",
									NULL );
	iMobileControl->SetDivider( 75 );
	iMobileControl->SetAlignment( B_ALIGN_RIGHT, B_ALIGN_LEFT );
	iMobileControl->SetFont( font );
	AddChild( iMobileControl );

	r.top = r.top + 25;
	r.bottom = r.top + 25;

	iEmailControl = new BTextControl( r,
									 "iEmailControl",
									 _T( "Email:" ),
									"",
									NULL );
	iEmailControl->SetDivider( 75 );
	iEmailControl->SetAlignment( B_ALIGN_RIGHT, B_ALIGN_LEFT );
	iEmailControl->SetFont( font );
	AddChild( iEmailControl );
	
	if( iNameControl->LockLooper() )
		{
		iNameControl->SetText( iPerson->GetName() );
		iNameControl->UnlockLooper();
		}
	if( iSurnameControl->LockLooper() )
		{
		iSurnameControl->SetText( iPerson->GetSurname() );
		iSurnameControl->UnlockLooper();
		}
	if( iNickControl->LockLooper() )
		{
		iNickControl->SetText( iPerson->GetNick() );
		iNickControl->UnlockLooper();
		}
	if( iDisplayControl->LockLooper() )
		{
		iDisplayControl->SetText( iPerson->GetDisplay() );
		iDisplayControl->UnlockLooper();
		}
	if( iMobileControl->LockLooper() )
		{
		iMobileControl->SetText( iPerson->GetEmail() );
		iMobileControl->UnlockLooper();
		}
	if( iEmailControl->LockLooper() )
		{
		iEmailControl->SetText( iPerson->GetEmail() );
		iEmailControl->UnlockLooper();
		}
	r = Bounds();
	r.left = r.right - 140;
	r.right = r.right - 75;
	r.top = r.bottom - 30;
	r.bottom = -5;
	BButton *button;
    button = new BButton( r, "ok button", _T( "OK" ), new BMessage( BUDDY_EDIT_OK ) );
    button->MakeDefault( true );
    AddChild( button );
    
    r = Bounds();
	r.left = r.right - 65;
	r.right = r.right -5;
	r.top = r.bottom - 30;
	r.bottom = -5;
    
    button = new BButton( r, "cancel button", _T( "Cancel" ), new BMessage( BUDDY_EDIT_CANCEL ) );
    AddChild( button );
	}

void BuddyEdit::MessageReceived( BMessage* aMessage )
	{
	switch( aMessage->what )
		{
		case BUDDY_EDIT_OK:
			{
			iPerson->SetName( iNameControl->Text() );
			iPerson->SetSurname( iSurnameControl->Text() );
			iPerson->SetNick( iNickControl->Text() );
			iPerson->SetDisplay( iDisplayControl->Text() );
			iPerson->SetMobile( iMobileControl->Text() );
			iPerson->SetEmail( iEmailControl->Text() );
			BMessenger( iWindow ).SendMessage( UPDATE_LIST );
			BMessenger( this ).SendMessage( B_QUIT_REQUESTED );
			break;
			}
		
		case BUDDY_EDIT_CANCEL:
			{
			BMessenger( this ).SendMessage( B_QUIT_REQUESTED );
			break;
			}
			
		default:
			BWindow::MessageReceived( aMessage );
		}
	}

void BuddyEdit::Show()
	{
    BWindow::Show();
	}
