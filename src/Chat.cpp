/*
	Chat.cpp
	Code: aljen <aljen@gumisie.org>
	Homepage: http://gadu.beos.pl
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <TextControl.h>
#include <ScrollView.h>
#include <TextView.h>
#include <Screen.h>
#include <String.h>
#include "Msg.h"
#include "Chat.h"
#include "Network.h"
#include "Main.h"
#include "Person.h"
#include <libgadu.h>

#define CHATWIN_RECT BRect(100,100,500,400)
#define CHATWIN_NAME ""

ChatWindow::ChatWindow( Network *aNetwork, MainWindow *aWindow, uin_t aWho )
	: BWindow( CHATWIN_RECT, CHATWIN_NAME,
			   B_FLOATING_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_NOT_ZOOMABLE )
	{
	iNetwork = aNetwork;
	iWindow	= aWindow;
	iWho = aWho;
	SetSizeLimits( 300, 2000, 200, 2000 );
	
	/*
		we're fixing a title and checking that we have that number in our people list
		if true, set to iDisplay
	*/
	Person *person;
	BString title = Title();
	BString *pe = NULL;
	for( int i = 0; i < iWindow->GetProfile()->GetUserlist()->GetList()->CountItems(); i++ )
		{
		person = ( Person* ) iWindow->GetProfile()->GetUserlist()->GetList()->ItemAt( i );
		if( aWho == person->iUIN )
			{
			pe = person->iDisplay;
			break;
			}
		}
	if( !pe )
		{
		pe = new BString();
		pe->SetTo( "[Nieznajomy]" );
		}

	title.Append( pe->String() );
	SetTitle( title.String());
	
	/* making a default background */
	BRect r = Bounds();
	BView *someView;
	someView = new BView( r, "some view", B_FOLLOW_ALL, B_WILL_DRAW );
	someView->SetViewColor( 60, 60, 60 );
	AddChild( someView );
	
	/* 'chat' at base of BTextView */
	r = someView->Bounds();
	r.InsetBy( 10, 10 );
	r.right -= B_V_SCROLL_BAR_WIDTH;
	r.bottom -= 25;
	BRect textRect = BRect( 5, 5, r.Width() - 5, r.Height() - 5 );
	iChat = new BTextView( r, "chat view", textRect, B_FOLLOW_ALL, B_WILL_DRAW );
	iChat->MakeEditable( false );
	iChat->SetStylable( true );
	BFont *font = new BFont( be_plain_font );
	font->SetSize( 15.0 );
	font->SetEncoding( B_ISO_8859_2 );
	iChat->SetFontAndColor( font );
	iScrollView = new BScrollView( "scroll view", iChat, B_FOLLOW_ALL, 0, false, true );
	someView->AddChild( iScrollView );
	iChat->SetViewColor( 70, 70, 70 );	
	/* new message edit box */
	r = someView->Bounds();
	r.InsetBy( 10, 10 );
	r.top = r.bottom - 15;
	iSayControl = new BTextControl( r, "say control", "", NULL, new BMessage( BEGG_SEND ), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_BOTTOM );
	iSayControl->MakeFocus( true );
	float width, height;
	iSayControl->GetPreferredSize( &width, &height );
	iSayControl->SetDivider( width / 2 );
	iSayControl->SetFont( font );
	someView->AddChild( iSayControl );
	}

void ChatWindow::MessageReceived( BMessage* aMessage )
	{
	switch( aMessage->what )
		{
		case SHOW_MESSAGE:
			{
			const char *msg;
			aMessage->FindString( "msg", &msg );
			time_t _now = time( NULL );
			struct tm *now = localtime( &_now );
			BString *str = NULL;
			BString *str2 = NULL;
			char *string = NULL;
			Person *person = NULL;
			for( int i = 0; i < iWindow->GetProfile()->GetUserlist()->GetList()->CountItems(); i++ )
				{
				person = ( Person* ) iWindow->GetProfile()->GetUserlist()->GetList()->ItemAt( i );
				if( iWho == person->iUIN )
					{
					str = new BString();
					str->SetTo( person->iDisplay->String() );
					break;
					}
				}
			if( !str )
				{
				str = new BString();
				*str << ( int32 ) iWho;
				}
			BFont *font = new BFont( be_plain_font );
			font->SetSize( 16.0 );
			font->SetEncoding( B_ISO_8859_2 );
			rgb_color yellow = { 255, 255, 0, 0 };
			rgb_color red = { 255, 0, 0, 0 };
			rgb_color white = { 255, 255, 255, 0 };
			string = ( char* ) calloc( strlen( "[00:00] " ), 1 );
			sprintf( string, "[%02d:%02d] ", now->tm_hour, now->tm_min);
			str2 = new BString();
			str2->SetTo( string );
			free( string );
//			str2 << "[" << (int32)now->tm_hour << ":" << now->tm_min << "] ";
			iChat->SetFontAndColor( iChat->TextLength(), iChat->TextLength() + str2->Length(), font, B_FONT_ALL, &yellow );
			iChat->Insert( iChat->TextLength(), str2->String(), str2->Length() );
			str->Append( ": " );

			iChat->SetFontAndColor( iChat->TextLength(), iChat->TextLength() + str->Length(), font, B_FONT_ALL, &red );
			iChat->Insert( iChat->TextLength(), str->String(), str->Length() );

			str2->SetTo( msg );
			str2->Append( "\n" );
			iChat->SetFontAndColor( iChat->TextLength(), iChat->TextLength() + str2->Length(), font, B_FONT_ALL, &white );
			iChat->Insert( iChat->TextLength(), str2->String(), str2->Length() );
			BScrollBar * scrollBar = iScrollView->ScrollBar( B_VERTICAL );
			if( scrollBar->LockLooper() )
				{
				float max,min;
				scrollBar->GetRange( &min, &max );
				scrollBar->SetValue( max );
				scrollBar->UnlockLooper();
				}
			delete str;
			delete str2;
			break;			
			}
			
		case BEGG_SEND:
			{
			if( iSayControl->LockLooper())
				{
				if( !(*iSayControl->Text() ) )
					{
					/* nothing to send */
					iSayControl->UnlockLooper();
					break;
					}
				
				/* first we add message into chat window */
				time_t _now = time( NULL );
				struct tm * now = localtime( &_now );
				BString str;
				BString str2;
				char *string;
				int id = iNetwork->GetIdent();

				BFont *font = new BFont( be_plain_font );
				font->SetSize( 16.0 );
				font->SetEncoding( B_ISO_8859_2 );
				rgb_color yellow = { 255, 255, 0, 0 };
				rgb_color green = { 0, 255, 0, 0 };
				rgb_color white = { 255, 255, 255, 0 };
				string = ( char* ) calloc( strlen( "[00:00] " ), 1 );
				sprintf( string, "[%02d:%02d] ", now->tm_hour, now->tm_min );
				str2.SetTo( string );
				free( string );
				iChat->SetFontAndColor( iChat->TextLength(), iChat->TextLength() + str2.Length(), font, B_FONT_ALL, &yellow );
				iChat->Insert( iChat->TextLength(), str2.String(), str2.Length() );

				str.SetTo( iWindow->GetProfile()->iProfileName->String() );
				str.Append( ": " );
				iChat->SetFontAndColor( iChat->TextLength(), iChat->TextLength() + str.Length(), font, B_FONT_ALL, &green );
				iChat->Insert( iChat->TextLength(), str.String(), str.Length() );

				str2.SetTo( iSayControl->Text() );
				str2.Append( "\n" );
				iChat->SetFontAndColor( iChat->TextLength(), iChat->TextLength() + str2.Length(), font, B_FONT_ALL, &white );
				iChat->Insert( iChat->TextLength(), str2.String(), str2.Length() );

//				string = (char*)calloc(strlen(" [00:00]\n%s\n") + 1 + fWindow->fProfil->fNazwaProfilu->Length() + strlen(fPowiedzControl->Text()), 1);
//				sprintf(string, "%s [%02d:%02d]\n%s\n", fWindow->fProfil->fNazwaProfilu->String(), teraz->tm_hour, teraz->tm_min, fPowiedzControl->Text());
//				fRozmowa->Insert(fRozmowa->TextLength(), string, strlen(string));
//				free(string);
				
				/* scroll down */
				BScrollBar * scrollBar = iScrollView->ScrollBar( B_VERTICAL );
				if( scrollBar->LockLooper() )
					{
					float max,min;
					scrollBar->GetRange( &min, &max );
					scrollBar->SetValue( max );
					scrollBar->UnlockLooper();
					}
				
				/* sending... */
				BMessage *newmessage;
				newmessage = new BMessage( SEND_MESSAGE );
				newmessage->AddInt32( "who", iWho );
				newmessage->AddString( "msg", iSayControl->Text());
				BMessenger( iNetwork ).SendMessage( newmessage );
				delete newmessage;
				/* clearing edit box */
				iSayControl->SetText( NULL );
				iSayControl->UnlockLooper();
				}
			break;
			}
			
		default:
			BWindow::MessageReceived( aMessage );
			break;
		}
	}

bool ChatWindow::QuitRequested()
	{
	BMessage *mesg = new BMessage( CLOSE_MESSAGE );
	mesg->AddPointer( "win", this );
	BMessenger( iNetwork ).SendMessage( mesg );
	delete mesg;
	return false;
	}

void ChatWindow::FrameResized( float width, float height )
	{
	BWindow::FrameResized( width, height );
	BRect	r = iChat->Bounds();
	r.InsetBy( 10, 10 );
	iChat->SetTextRect( r );
	BScrollBar *scrollBar = iScrollView->ScrollBar( B_VERTICAL );
	if( scrollBar->LockLooper() )
		{
		float min, max;
		scrollBar->GetRange( &min, &max );
		scrollBar->SetValue( max );
		scrollBar->UnlockLooper();
		}
	}

void ChatWindow::Show()
	{
	/* focusing window */
	BPoint point;
	point.x = iWindow->Frame().left + 
				( iWindow->Frame().Width() - Frame().Width() ) / 2;
	point.y = iWindow->Frame().top +
				( iWindow->Frame().Height() - Frame().Height() ) / 2;
	BScreen _screen;
	BRect screen = _screen.Frame();
	point.x = MIN( point.x, screen.right - Frame().Width() );
	point.x = MAX( point.x, screen.left );
	point.y = MIN( point.y, screen.bottom - Frame().Height() );
	point.y = MAX( point.y, screen.top );
	MoveTo( point );
	iSayControl->MakeFocus();
	BWindow::Show();
	}

void ChatWindow::WindowActivated( bool aActivated )
	{
	if( aActivated )
		{
		iSayControl->MakeFocus( true );
		}
	BWindow::WindowActivated( aActivated );
	}
