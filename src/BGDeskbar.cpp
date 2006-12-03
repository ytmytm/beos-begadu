/*
 * ============================================================================
 *  Name     : BGDeskbar from BGDeskbar.h
 *  Part of  : BeGadu
 *  Authors  : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Implementation notes:
 *		BGDeskbar replicant
 *  Version  : 1.2
 * ============================================================================
 */

#include <Alert.h>
#include <Bitmap.h>
#include <Deskbar.h>
#include <FindDirectory.h>
#include <InterfaceDefs.h>
#include <Menu.h>
#include <MenuItem.h>
#include <Message.h>
#include <Path.h>
#include <PopUpMenu.h>
#include <Resources.h>
#include <Roster.h>
#include <Screen.h>
#include <String.h>
#include <stdio.h>

#include "BGDeskbar.h"
#include "Debug.h"
#include "GaduMenuItem.h"
#include "GfxStuff.h"
#include "Main.h"
#include "Msg.h"
#include "ProfileWizard.h"

extern "C" _EXPORT BView* instantiate_deskbar_item();

BView *instantiate_deskbar_item( void )
	{
	fprintf( stderr, "instantiate_deskbar_item()\n" );
	return new BGDeskbar();
	}

BGDeskbar::BGDeskbar() : BView( BRect( 0, 0, 15, 15),
								"BGDeskbar",
								B_FOLLOW_LEFT | B_FOLLOW_TOP,
								B_WILL_DRAW | B_PULSE_NEEDED )
	{
	fprintf( stderr, "BGDeskbar()\n" );
	iIcon = NULL;
	iIconAvail = NULL;
	iIconAvailDescr = NULL;
	iIconBusy = NULL;
	iIconBusyDescr = NULL;
	iIconInvisible = NULL;
	iIconInvisibleDescr = NULL;
	iIconNotAvail = NULL;
	iIconNotAvailDescr = NULL;
	iIconQuit = NULL;
	iIconNewMessage = NULL;
	iMenu = NULL;
	}

BGDeskbar::BGDeskbar( BMessage *aMessage ) : BView( aMessage )
	{
	fprintf( stderr, "BGDeskbar( aMessage )\n" );
	iIcon = NULL;
	iIconAvail = NULL;
	iIconAvailDescr = NULL;
	iIconBusy = NULL;
	iIconBusyDescr = NULL;
	iIconInvisible = NULL;
	iIconInvisibleDescr = NULL;
	iIconNotAvail = NULL;
	iIconNotAvailDescr = NULL;
	iIconQuit = NULL;
	iIconNewMessage = NULL;
	iMenu = NULL;
	Initialize();
	}

BGDeskbar::~BGDeskbar()
	{
	fprintf( stderr, "BGDeskbar::~BGdeskbar()\n" );
	if( iIconAvail )
		{
		delete iIconAvail;
		iIconAvail = NULL;
		}
	if( iIconAvailDescr )
		{
		delete iIconAvailDescr;
		iIconAvailDescr = NULL;
		}
	if( iIconBusy )
		{
		delete iIconBusy;
		iIconBusy = NULL;
		}
	if( iIconBusyDescr )
		{
		delete iIconBusyDescr;
		iIconBusyDescr = NULL;
		}
	if( iIconInvisible )
		{
		delete iIconInvisible;
		iIconInvisible = NULL;
		}
	if( iIconInvisibleDescr )
		{
		delete iIconInvisibleDescr;
		iIconInvisibleDescr = NULL;
		}
	if( iIconNotAvail )
		{
		delete iIconNotAvail;
		iIconNotAvail = NULL;
		}
	if( iIconNotAvailDescr )
		{
		delete iIconNotAvailDescr;
		iIconNotAvailDescr = NULL;
		}
	if( iIconQuit )
		{
		delete iIconQuit;
		iIconQuit = NULL;
		}
	if( iIconNewMessage )
		{
		delete iIconNewMessage;
		iIconNewMessage = NULL;
		}
	if( iIcon )
		{
//		delete iIcon;
		iIcon = NULL;
		}
	if( iMenu )
		{
		delete iMenu;
		iMenu = NULL;
		}
	}

void BGDeskbar::Initialize()
	{
	fprintf( stderr, "BGDeskbar::Initialize()\n" );
	BRoster roster;
	entry_ref ref;
	BFile resfile;
	if( roster.IsRunning( "application/x-vnd.BeGadu" ) )
		{
		fprintf( stderr, "BGDeskbar: Jest BeGadu :D\n" );
		roster.FindApp( "application/x-vnd.BeGadu", &ref );
		resfile.SetTo( &ref, B_READ_ONLY );	
		iResources.SetTo( &resfile );
		iIconAvail = GetBitmap( "online" );
		iIconBusy = GetBitmap( "busy" );
		iIconInvisible = GetBitmap( "invisible" );
		iIconNotAvail = GetBitmap( "offline" );
		iIconAvailDescr = GetBitmap( "online_d" );
		iIconBusyDescr = GetBitmap( "busy_d" );
		iIconInvisibleDescr = GetBitmap( "invisible_d" );
		iIconNotAvailDescr = GetBitmap( "offline_d" );
		iIconNewMessage = GetBitmap( "message" );
		iIconQuit = GetBitmap( "quit" );
		iIcon = iIconNotAvail;
		}
	else
		{
		fprintf(stderr, "BGDeskbar: Nie ma BeGadu :(\n" );
		be_roster->Launch( "application/x-vnd.BeGadu" );
		}
	iMenu = new BPopUpMenu( "BGDeskbarMenu", true, true );
	GaduMenuItem *availItem = new GaduMenuItem( iIconAvail, "Dostepny", new BMessage( SET_AVAIL ) );
	iMenu->AddItem( availItem );
	GaduMenuItem *busyItem = new GaduMenuItem( iIconBusy, "Zaraz wracam", new BMessage( SET_BRB ) );
	iMenu->AddItem( busyItem );
	GaduMenuItem *invisibleItem = new GaduMenuItem( iIconInvisible, "Niewidoczny", new BMessage( SET_INVIS ) );
	iMenu->AddItem( invisibleItem );
	GaduMenuItem *notavailItem = new GaduMenuItem( iIconNotAvail, "Niedostepny", new BMessage( SET_NOT_AVAIL ) );
	iMenu->AddItem( notavailItem );
	GaduMenuItem *descrItem = new GaduMenuItem( iIconAvailDescr, "Zmien opis", new BMessage( SET_DESCRIPTION ) );
	iMenu->AddItem( descrItem );
	iMenu->AddSeparatorItem();
	GaduMenuItem *aboutItem = new GaduMenuItem( iIconAvail, "O BeGadu..", new BMessage( BEGG_ABOUT ) );
	iMenu->AddItem( aboutItem );
	GaduMenuItem *quitItem = new GaduMenuItem( iIconQuit, "Zakoncz", new BMessage( BEGG_QUIT ) );
	iMenu->AddItem( quitItem );

//	if( iMenu->SetTargetForItems( this ) == B_OK )
//	{
//		fprintf( stderr, "iMenu->SetTargetForItems( B_OK )\n", roster );
//	}
//	else
//	{
//		fprintf( stderr, "iMenu->SetTargetForItems( FAILED )\n" );
//	}
	}

BArchivable* BGDeskbar::Instantiate( BMessage *aData )
	{
	if( !validate_instantiation( aData, "BGDeskbar" ) )
		{
		return NULL;
		}
	return ( new BGDeskbar( aData ) );
	}

status_t BGDeskbar::Archive( BMessage *aData, bool aDeep = true) const
	{
	BView::Archive( aData, aDeep );
	aData->AddString( "add_on", "application/x-vnd.BeGadu" );
	return B_NO_ERROR;
	}

void BGDeskbar::MessageReceived( BMessage *aMessage )
	{
	switch( aMessage->what )
		{
		case BGDESKBAR_CHSTATE:
			{
			fprintf( stderr, "BGDeskbar::MessageReceived( BGDESKBAR_CHSTATE );\n" );
			int16 status;
			if( aMessage->FindInt16( "iStatus", &status ) != B_OK )
				{
				fprintf( stderr, "\tcan't find 'iStatus' in message...\n" );
				break;
				}
			else
				{
				switch( status )
					{
					case GG_STATUS_AVAIL:
						{
						iIcon = iIconAvail;
						break;
						}

					case GG_STATUS_BUSY:
						{
						iIcon = iIconBusy;
						break;
						}

					case GG_STATUS_INVISIBLE:
						{
						iIcon = iIconInvisible;
						break;
						}

					case GG_STATUS_NOT_AVAIL:
						{
						iIcon = iIconNotAvail;
						break;
						}

					case GG_STATUS_AVAIL_DESCR:
						{
						iIcon = iIconAvailDescr;
						break;
						}

					case GG_STATUS_BUSY_DESCR:
						{
						iIcon = iIconBusyDescr;
						break;
						}

					case GG_STATUS_INVISIBLE_DESCR:
						{
						iIcon = iIconInvisibleDescr;
						break;
						}

					case GG_STATUS_NOT_AVAIL_DESCR:
						{
						iIcon = iIconNotAvailDescr;
						break;
						}
					}
				}
			break;
			}
		/* handling messages */
		default:
			BView::MessageReceived( aMessage );
		}
	}


void BGDeskbar::Draw( BRect aRect )
	{
	SetHighColor( Parent()->ViewColor() );
	FillRect( Bounds() );
	SetDrawingMode( B_OP_OVER );

	if( iIcon )
		{
		SetDrawingMode( B_OP_ALPHA );
		SetLowColor( 0, 0, 0, 0 );
		SetViewColor( B_TRANSPARENT_32_BIT );
		DrawBitmap( iIcon, BPoint( 0, 0 ) );
		}
	else
		{
		SetHighColor( 110, 110, 110 );
		FillRect( Bounds() );
		}
	}

void BGDeskbar::Pulse()
	{
	/* temporary... */
	Invalidate();
	}

void BGDeskbar::MouseDown( BPoint aWhere )
	{
	fprintf( stderr, "BGDeskbar: MouseDown( aWhere )\n" );
	unsigned long buttons;
	if( LockLooper() )
		{
		GetMouse( &aWhere, &buttons, false );
		UnlockLooper();
		}
	if( buttons & B_PRIMARY_MOUSE_BUTTON )
		{
		fprintf( stderr, "BGDeskbar: MouseDown( B_PRIMARY_MOUSE_BUTTON )\n" );
		BMessenger( "application/x-vnd.BeGadu" ).SendMessage( new BMessage( SHOW_MAIN_WINDOW ) );
//		if( iWindow->LockLooper() )
//		{
//			if( iWindow->IsHidden() )
//				iWindow->Show();
//			else
//				iWindow->Activate();
//			iWindow->UnlockLooper();
//		}
		}
	else if( buttons & B_SECONDARY_MOUSE_BUTTON )
		{
		fprintf( stderr, "BGDeskbar: MouseDown( B_SECONDARY_MOUSE_BUTTON )\n" );
		GaduMenuItem *selectedItem = (GaduMenuItem*) iMenu->Go( ConvertToScreen( aWhere ), false, true );
		if( selectedItem )
			{
			BMessenger( "application/x-vnd.BeGadu" ).SendMessage( selectedItem->Message() );
			}
		}
	}

void BGDeskbar::AttachedToWindow()
	{
	fprintf( stderr, "BGDeskbar::AttachedToWindow()\n" );
	snooze( 500*100 );
	BMessage* message = new BMessage( ADD_MESSENGER );
	message->AddMessenger( "messenger", BMessenger( this ) );
	BMessenger( "application/x-vnd.BeGadu" ).SendMessage( message );
	delete message;
	/* temporary empty */
	BView::AttachedToWindow();
	}

void BGDeskbar::DetachedFromWindow()
	{
	fprintf( stderr, "BGDeskbar::DetachedFromWindow()\n" );
	/* temporary empty */
	BView::DetachedFromWindow();
	}

void BGDeskbar::Remove()
	{
	}

BBitmap *BGDeskbar::GetBitmap( const char* aName )
	{
	BBitmap 	*bitmap = NULL;
	size_t 		len = 0;
	status_t 	error;	

	const void *data = iResources.LoadResource( 'BBMP', aName, &len );

	BMemoryIO stream( data, len );
	
	BMessage archive;
	error = archive.Unflatten( &stream );
	if( error != B_OK )
		return NULL;
	bitmap = new BBitmap( &archive );
	if( !bitmap )
		return NULL;

	if( bitmap->InitCheck() != B_OK )
		{
		delete bitmap;
		return NULL;
		}
	
	return bitmap;
	}
