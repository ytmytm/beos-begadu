
#include <Alert.h>
#include <Beep.h>
#include <Bitmap.h>
#include <MenuBar.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <Roster.h>
#include <StringView.h>
#include <ScrollView.h>
#include <String.h>
#include <StorageKit.h>

#include "About.h"
#include "BuddyEdit.h"
#include "GaduListView.h"
#include "GaduListItem.h"
#include "GaduMenuItem.h"
#include "GfxStuff.h"
#include "Main.h"
#include "Msg.h"
#include "Network.h"
#include "Preferences.h"
#include "Person.h"
#include "Description.h"

#include "globals.h"

#define MAINWINDOW_RECT BRect(50,50,300,350)
#define MAINWINDOW_NAME	"BeGadu " VERSION

MainWindow::MainWindow( BString* aProfile )
 	: BWindow( MAINWINDOW_RECT, MAINWINDOW_NAME, B_TITLED_WINDOW, 
 			   B_NOT_ZOOMABLE | B_ASYNCHRONOUS_CONTROLS )
	{
	DEBUG_TRACE( "MainWindow::MainWindow()\n"  );

	iPreferencesWindow = NULL;
	iProfile = new Profile();
	iProfile->Load( aProfile );
	SetTitle( aProfile->String() );
	BRoster roster;
	entry_ref ref;
	BFile resfile;
	roster.FindApp( APP_MIME, &ref );
	resfile.SetTo( &ref, B_READ_ONLY );
	iResources.SetTo( &resfile );
	LoadIcons();
	SetSizeLimits( 250, 600, 300, 600 );
	iNetwork = new Network( iProfile );
	iNetwork->GotWindow( this );
	iListItems = new List( 512 );
	if( iProfile->GetRect().IsValid() )
		MoveTo( iProfile->GetRect().left, iProfile->GetRect().top );
	/* setting menu */
	BRect r = Bounds();
	iMenuBar = new BMenuBar( r, "menu bar" );
	iMenu = new BMenu( _T("Menu") );
	iProfilesMenu = new BMenu( _T("Profiles") );

	/* searching for user profiles */
	BPath path;
	BEntry entry;
	BMenuItem *item = NULL;
	char name[ B_FILE_NAME_LENGTH ];
	BString *nameString;

	find_directory( B_USER_SETTINGS_DIRECTORY, &path );
	BDirectory * bg_conf = new BDirectory( path.Path() );
	path.Append( "BeGadu/Profiles" );
	bg_conf->SetTo( path.Path() );

	/* filling list */

	if( bg_conf->CountEntries() > 0 ) {
		bg_conf->Rewind();
		while( bg_conf->GetNextEntry( &entry ) == B_OK ) {
			if( entry.IsDirectory() ) {
				entry.GetName( name );
				nameString = new BString( name );
				/* adding item to menu */
				item = new BMenuItem( nameString->String(), new BMessage( PROFILE_SWITCH ) );
				iProfilesMenu->AddItem( item );
				/* checking if that profile is last used profile */
				if( nameString->Compare( aProfile->String() ) == 0 ) {
					/* selecting this item as active */
					item->SetMarked( true );
				}
			}
		}
	}
	iProfilesMenu->AddSeparatorItem();
	item = new BMenuItem( _T( "New" B_UTF8_ELLIPSIS), new BMessage( OPEN_PROFILE_WIZARD ) );
	iProfilesMenu->AddItem( item );

	iProfileItem = new BMenuItem( iProfilesMenu, NULL );
	iMenu->AddItem( iProfileItem );
	iMenu->AddSeparatorItem();

	iUserlistMenu = new BMenu( _T("Userlist") );
	iListImport = new BMenuItem( _T("Get from server"), new BMessage( BEGG_IMPORT_LIST ) );
	iListExport = new BMenuItem( _T("Send to server"), new BMessage( BEGG_EXPORT_LIST ) );
	iUserlistMenu->AddItem( iListImport );
	iUserlistMenu->AddItem( iListExport );
	iListMenu = new BMenuItem( iUserlistMenu, NULL );
	iMenu->AddItem( iListMenu );
	iMenu->AddSeparatorItem();

	iPreferences = new BMenuItem( _T("Preferences"), new BMessage( BEGG_PREFERENCES ) );
	iMenu->AddItem( iPreferences );
	iAbout = new BMenuItem( _T("About" B_UTF8_ELLIPSIS), new BMessage( BEGG_ABOUT ) );
	iMenu->AddItem( iAbout );
	iMenu->AddItem( new BMenuItem( _T("Quit"), new BMessage( BEGG_QUIT ) ) );
	iMenuBar->AddItem( iMenu );
	AddChild( iMenuBar );

	r.top = iMenuBar->Bounds().bottom ;
//	r.bottom = r.top + 32;
//	fIconsView = new BView( BRect(r.left, r.top, r.right, r.bottom), "IconsView", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP , B_FULL_UPDATE_ON_RESIZE);
//	fIconsView->SetViewColor(100,100,100);
//	AddChild(fIconsView);

//	BBitmap *stan1 = LoadGFX("/boot/apps/BeGadu/images/1.png");
//	BBitmap *stan2 = LoadGFX("/boot/apps/BeGadu/images/2.png");
//	IconControl *iconcontrol = new IconControl(BRect(r.left, r.top + 3, r.left + 30, r.top + 30), "iconcontrol", stan1, stan2, new BMessage(B_QUIT_REQUESTED));
//	AddChild(iconcontrol);

//	r = Bounds();
//	r.top = fIconsView->Bounds().bottom + menuBar->Bounds().bottom;

	iGaduView = new BView( r, "iGaduView", B_FOLLOW_ALL, B_FRAME_EVENTS | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE | B_SUBPIXEL_PRECISE );
	iGaduView->SetViewColor( 225, 225, 225 ); // 90, 90, 90
	AddChild( iGaduView );

	/* list */
	r = iGaduView->Bounds();
	r.right -= B_V_SCROLL_BAR_WIDTH;
	r.bottom -= 30;

	iListView = new GaduListView( this, r, "iListView" );
	iListView->SetViewColor( 255, 255, 255 );
	BFont *font = new BFont( be_plain_font );
	font->SetSize( 14.0 );
	iListView->SetFont( font );
	iScrollView = new BScrollView( "iScrollView", iListView, B_FOLLOW_ALL, B_SUBPIXEL_PRECISE | B_FRAME_EVENTS | B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE, false, true, B_PLAIN_BORDER );
	iGaduView->AddChild( iScrollView );

	/* we must know if user select/open person */
	iListView->SetSelectionMessage( new BMessage( BEGG_PERSON_SELECTED ) );
	iListView->SetInvocationMessage( new BMessage( BEGG_PERSON_ACTION ) );

	/* adding status */
	r = iGaduView->Bounds();
	r.top = iListView->Bounds().bottom + 5;
	r.left = r.left + 5;
	r.right = r.left + r.Width()/2;

	iStatusMenu = new BPopUpMenu( "change_status" );
	iAvail			= new GaduMenuItem( iIconAvail, _T("Available"), new BMessage( SET_AVAIL ) );
	iBRB			= new GaduMenuItem( iIconBRB, _T("Be right back"), new BMessage( SET_BRB ) );
	iInvisible		= new GaduMenuItem( iIconInvisible, _T("Invisible"), new BMessage( SET_INVIS ) );
	iNotAvail		= new GaduMenuItem( iIconNotAvail, _T("Not available"), new BMessage( SET_NOT_AVAIL ) );
	iDescr			= new GaduMenuItem( iIconAvailDescr, _T("Description" B_UTF8_ELLIPSIS), new BMessage( CHANGE_DESCRIPTION ) );	

	iStatusMenu->AddItem( iAvail );
	iStatusMenu->AddItem( iBRB );
	iStatusMenu->AddItem( iInvisible );
	iStatusMenu->AddItem( iNotAvail );
	iStatusMenu->AddItem( iDescr );
	iNotAvail->SetMarked( true );

	iStatus = new BMenuField( r, "iStatus", _T("Status:"), iStatusMenu, B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, B_WILL_DRAW | B_NAVIGABLE | B_FRAME_EVENTS );
	iStatus->SetDivider( 50 );
	iGaduView->AddChild( iStatus );

// ???
//	add_system_beep_event( ONLINE_SOUND );
//	add_system_beep_event( MESSAGE_SOUND );

	r.OffsetBy(r.Width(), -8);
	iGaduView->AddChild(iConnStatus = new BStringView(r, "iConnStatus", NULL, B_FOLLOW_RIGHT|B_FOLLOW_BOTTOM, B_WILL_DRAW | B_NAVIGABLE | B_FRAME_EVENTS ));
	iConnStatus->SetAlignment(B_ALIGN_RIGHT);

	if( iProfile->GetUserlist()->IsValid() ) {
		iListItems = iProfile->GetUserlist()->GetList();
		BMessenger( this ).SendMessage( UPDATE_LIST );
	}

	fprintf( stderr, "Profile %s loaded.\n", iProfile->GetProfileName() );
	fprintf( stderr, "profile status:[%i],desc=[%s]\n",iProfile->GetAutoStatus(),iProfile->GetDescription());

	if( iProfile->GetAutoStatus() != GG_STATUS_NOT_AVAIL ) {
		if( iNetwork->Session() )
			gg_change_status( iNetwork->Session(), iProfile->GetAutoStatus() );
		else
			iNetwork->Login();
	}
}

MainWindow::~MainWindow() {
	if( iProfile )
		delete iProfile;
	if( iIconAvail )
		delete iIconAvail;
	if( iIconBRB )
		delete iIconBRB;
	if( iIconInvisible )
		delete iIconInvisible;
	if( iIconNotAvail )
		delete iIconNotAvail;
	if( iIconAvailDescr )
		delete iIconAvailDescr;
	if( iIconBRBDescr )
		delete iIconBRBDescr;
	if( iIconInvisibleDescr )
		delete iIconInvisibleDescr;
	if( iIconNotAvailDescr )
		delete iIconNotAvailDescr;
}

bool MainWindow::QuitRequested() {
	DEBUG_TRACE( "MainWindow::QuitRequested()\n" );
	/* saving profile settings */
	if( LockLooper() ) {
		if( !IsHidden() )
			Hide();
		UnlockLooper();
	}
	return false;
}

void MainWindow::MessageReceived( BMessage* aMessage ) {
	switch( aMessage->what ) {
		case PROFILE_SWITCH:
			{
			DEBUG_TRACE( "MainWindow::MessageReceived( PROFILE_SWITCH )\n" );
			BMenuItem *item = NULL;
			int32 index = 0;
			aMessage->FindInt32( "index", &index );
			aMessage->FindPointer( "source", ( void** ) &item );
			BString *profile = new BString( "" );
			profile->SetTo( item->Label() );
			fprintf(stderr, "Switching profile to: [%s]\n",item->Label());
			BMessage *message = new BMessage( PROFILE_SWITCH );
			message->AddString( "iProfileName", *profile );
			BMessenger( be_app ).SendMessage( message );
			break;
			}
		case BUDDY_EDIT:
			{
			DEBUG_TRACE( "MainWindow::MessageReceived( BUDDY_EDIT )\n" );
			Person* person = NULL;
			GaduListItem *who = ( GaduListItem* ) iListView->ItemAt( iListView->CurrentSelection() );
			for( int i = 0; i < iProfile->GetUserlist()->GetList()->CountItems(); i++ ) {
				person = ( Person* ) iProfile->GetUserlist()->GetList()->ItemAt( i );
				if( strcmp( person->GetDisplay(), who->iName->String() ) == 0)
					break;
			}
			if( person->GetUIN() == iProfile->GetUIN() )
				break;
			BuddyEdit *buddyEdit = new BuddyEdit( this, BRect( 50, 50, 300, 250), person );
			buddyEdit->Show();
			break;
			}
		case CHANGE_DESCRIPTION:
			{
			DEBUG_TRACE( "MainWindow::MessageReceived( CHANGE_DESCRIPTION )\n" );
			Description* desc = new Description( this, BRect( 10, 10, 20, 20 ), &iResources );
			desc->Show();
			break;
			}
		case BEGG_PREFERENCES:
			DEBUG_TRACE( "MainWindow::MessageReceived( BEGG_PREFERENCES )\n" );
			if( iPreferencesWindow == NULL ) {
				iPreferencesWindow = new Preferences( iProfile, this, BRect( 10, 10, 20, 20 ), &iResources );
			}
			// XXX below - doesn't work for showing
			if( iPreferencesWindow->LockLooper() ) {
				if( iPreferencesWindow->IsHidden() )
					iPreferencesWindow->Show();
				else
					iPreferencesWindow->Activate();
				iPreferencesWindow->UnlockLooper();
			}
			break;
		case PREFERENCES_CLOSE:
			DEBUG_TRACE( "MainWindow::MessageReceived( PREFERENCES_CLOSE )\n" );
			if( iPreferencesWindow )
				delete iPreferencesWindow;
			break;
		case BEGG_ABOUT:
			{
			DEBUG_TRACE( "MainWindow::MessageReceived( BEGG_ABOUT )\n" );
			AboutWindow *about = new AboutWindow();
			about->Show();
			break;
			}
		case SHOW_MAIN_WINDOW:
			DEBUG_TRACE( "MainWindow::MessageReceived( SHOW_MAIN_WINDOW )\n" );
			if( LockLooper() ) {
				if( IsHidden() )
					Show();
				else
					Activate();
				UnlockLooper();
			}
			break;
		case PREFERENCES_SWITCH:
			DEBUG_TRACE( "MainWindow::MessageReceived( PREFERENCES_SWITCH )\n" );
			BMessenger( iPreferencesWindow ).SendMessage( aMessage );
			break;
		case BEGG_PERSON_SELECTED:
			DEBUG_TRACE( "MainWindow::MessageReceived( BEGG_PERSON_SELECTED )\n" );
			break;
		case BEGG_PERSON_ACTION:
			{
			DEBUG_TRACE( "MainWindow::MessageReceived( BEGG_PERSON_ACTION )\n" );
			if( iListView->CurrentSelection() < 0 )
				break;
			Person* person = NULL;
			GaduListItem *who = ( GaduListItem* ) iListView->ItemAt( iListView->CurrentSelection() );
			for( int i = 0; i < iProfile->GetUserlist()->GetList()->CountItems(); i++ ) {
				person = ( Person* ) iProfile->GetUserlist()->GetList()->ItemAt( i );
				if( strcmp( person->GetDisplay(), who->iName->String() ) == 0)
					break;
			}
			if( person->GetUIN() == iProfile->GetUIN() )
				break;
			BMessage *message = new BMessage( OPEN_MESSAGE );
			message->AddInt32( "who", person->GetUIN() );
			BMessenger( iNetwork ).SendMessage( message );		
			delete message;
			break;
			}
		case SET_AVAIL:
			fprintf( stderr, "MainWindow::MessageReceived( SET_AVAIL )\n" );
			ChangeStatus(GG_STATUS_AVAIL, new BString(""));
			UpdateBGDeskBar();
			break;
		case SET_BRB:
			fprintf( stderr, "MainWindow::MessageReceived( SET_BRB )\n" );
			ChangeStatus(GG_STATUS_BUSY, new BString(""));
			UpdateBGDeskBar();
			break;
		case SET_INVIS:
			fprintf( stderr, "MainWindow::MessageReceived( SET_INVIS )\n" );
			ChangeStatus(GG_STATUS_INVISIBLE, new BString(""));
			UpdateBGDeskBar();
			break;
		case SET_NOT_AVAIL:
			fprintf( stderr, "MainWindow::MessageReceived( SET_NOT_AVAIL )\n" );
			iNetwork->SetDescription( new BString( "" ) );
			if( iNetwork->Session() ) {
				iNetwork->Logout();
			}
			UpdateBGDeskBar();
			break;
		case SET_DESCRIPTION:
			{
			DEBUG_TRACE(  "MainWindow::MessageReceived( SET_DESCRIPTION )\n" );
			int8 status = 0;
			BString *description = new BString( "" );
			aMessage->FindInt8( "iStatus", (int8*) &status );
			aMessage->FindString( "iDescription", description );
			if( status == GG_STATUS_NOT_AVAIL || status == GG_STATUS_NOT_AVAIL_DESCR ) {
				if( iNetwork->Session() ) {
					gg_change_status_descr( iNetwork->Session(), status, description->String() );
					iNetwork->SetStatus( status );
					iNetwork->SetDescription( description );
					iNetwork->Logout();
				}
			} else {
				if( iNetwork->Session() ) {
					gg_change_status_descr( iNetwork->Session(), status, description->String() );
					iNetwork->SetStatus( status );
					iNetwork->SetDescription( description );
					BMessenger( this ).SendMessage( UPDATE_LIST );
				} else {
					iNetwork->Login( status, description );
				}
			}
			UpdateBGDeskBar();
			break;
			}
		case UPDATE_STATUS:
			DEBUG_TRACE( "MainWindow::MessageReceived( UPDATE_STATUS )\n" );
			switch( iNetwork->GetStatus() ) {
				case GG_STATUS_NOT_AVAIL:
					iNotAvail->SetMarked( true );
			 		break;
				case GG_STATUS_INVISIBLE:
					iInvisible->SetMarked( true );
					break;
				case GG_STATUS_BUSY:
					iBRB->SetMarked( true );
					break;
				case GG_STATUS_AVAIL:
					iAvail->SetMarked( true );
					break;
				case GG_STATUS_AVAIL_DESCR:
				case GG_STATUS_BUSY_DESCR:
				case GG_STATUS_INVISIBLE_DESCR:
				case GG_STATUS_NOT_AVAIL_DESCR:
					iDescr->SetMarked( true );
					break;
				default:
					break;
			}
			break;
		case UPDATE_LIST:
			{
			DEBUG_TRACE( "MainWindow::MessageReceived( UPDATE_LIST )\n" );
			List *listAvail = new List( 128 );
			List *listNotAvail = new List( 128 );

			GaduListItem *g = NULL;
			Person	 *p = NULL;
			for( int i = 0; i < iProfile->GetUserlist()->GetList()->CountItems(); i++ ) {
				p = ( Person* ) iProfile->GetUserlist()->GetList()->ItemAt( i );
				if( p->GetUIN() == iProfile->GetUIN() ) {
					if( iNetwork->GetStatus() == GG_STATUS_NOT_AVAIL )
						g = new GaduListItem( p->GetDisplay(), iNetwork->GetStatus(), "", &iResources );
					else
						g = new GaduListItem( p->GetDisplay(), iNetwork->GetStatus(), iNetwork->iDescription->String(), &iResources );

					if( iNetwork->GetStatus() == GG_STATUS_NOT_AVAIL || iNetwork->GetStatus() == GG_STATUS_NOT_AVAIL_DESCR ||
						iNetwork->GetStatus() == GG_STATUS_INVISIBLE || iNetwork->GetStatus() == GG_STATUS_INVISIBLE_DESCR )
						listNotAvail->AddItem( g );
					else
						listAvail->AddItem( g );
				} else {
					if( iNetwork->GetStatus() == GG_STATUS_NOT_AVAIL || iNetwork->GetStatus() == GG_STATUS_NOT_AVAIL_DESCR )
						g = new GaduListItem( p->GetDisplay(), p->GetStatus(), "", &iResources );
					else
						g = new GaduListItem( p->GetDisplay(), p->GetStatus(), p->GetDescription(), &iResources );

					if( p->GetStatus() == GG_STATUS_NOT_AVAIL || p->GetStatus() == GG_STATUS_NOT_AVAIL_DESCR )
						listNotAvail->AddItem( g );
					else
						listAvail->AddItem( g );
				}
			}
			iListView->MakeEmpty();
			listAvail->SortItems( SortUsers );
			iListView->AddList( listAvail );
			listNotAvail->SortItems( SortUsers );
			iListView->AddList( listNotAvail );
			iListView->Invalidate();
			BScrollBar* sb = iScrollView->ScrollBar( B_VERTICAL );
			float listHeight = iScrollView->Bounds().bottom - iScrollView->Bounds().top;
			float itemsHeight = iListView->CountItems() * 35;
			float propH = listHeight / itemsHeight;
			float rangeH = itemsHeight - listHeight;
			sb->SetProportion( propH );
			sb->SetRange( 0, rangeH );
			sb->SetSteps( listHeight / 8.0, listHeight / 2.0 );
			delete listAvail;
			delete listNotAvail;
			break;
			}
		case BEGG_EXPORT_LIST:
			DEBUG_TRACE( "MainWindow::MessageReceived( BEGG_EXPORT_LIST )\n" );
			if( iNetwork->Session() )
				iProfile->GetUserlist()->Export( iNetwork->Session(), iProfile->GetUserlist()->GetList() );
			else {
				BAlert *alert = new BAlert( _T("Userlist"), _T("You must be connected to export contact list"), _T("OK") );
				alert->Go();
			}
			break;
		case BEGG_IMPORT_LIST:
			DEBUG_TRACE( "MainWindow::MessageReceived( BEGG_IMPORT_LIST )\n" );
			if( iNetwork->Session() )
				iProfile->GetUserlist()->Import( iNetwork->Session(), iProfile->GetUserlist()->GetList() );
			else {
				BAlert *alert = new BAlert( _T("Userlist"), _T("You must be connected to import contact list"), _T("OK") );
				alert->Go();
			}
			break;
		case SET_CONN_DESCRIPTION:
			{
			BString str;
			DEBUG_TRACE( "MainWindow::MessageReceived( SET_CONN_DESCRIPTION )\n" );
			if (aMessage->FindString("desc",&str) == B_OK) {
				iConnStatus->SetText(str.String());
			} else {
//				printf("NO MESSAGE!\n");
			}
			break;
			}
		case BGDESKBAR_CHSTATE:
			DEBUG_TRACE( "MainWindow::MessageReceived( BGDESKBAR_CHSTATE )\n" );
			iDeskbarMessenger.SendMessage( aMessage );
			break;
		case OPEN_PROFILE_WIZARD:
			BMessenger( be_app ).SendMessage( OPEN_PROFILE_WIZARD );
			break;
		case CLOSE_MAIN_WINDOW:
			DEBUG_TRACE( "MainWindow::MessageReceived( CLOSE_MAIN_WINDOW )\n" );
			iProfile->SetRect( Frame() );
			iProfile->Save();
			/* cleaning up ;D */
			iNetwork->GotWindow( NULL );
			iNetwork->Quit();
			break;
		case BEGG_QUIT:
			DEBUG_TRACE( "MainWindow::MessageReceived( BEGG_QUIT )\n" );
			// save last status & description
			if (iNetwork->Session()) {
				iProfile->SetAutoStatus(iNetwork->GetStatus());
				iProfile->SetDescription(iNetwork->iDescription->String());
				printf("profile descr set to: [%s]\n",iNetwork->iDescription->String());
			}
			//
			iProfile->SetRect( Frame() );
			iProfile->Save();
			/* cleaning up ;D */
			iNetwork->GotWindow( NULL );
			iNetwork->Quit();
			BMessenger( be_app ).SendMessage( B_QUIT_REQUESTED );
			break;
		default:
			BWindow::MessageReceived( aMessage );
			break;
		}
}

void MainWindow::ChangeStatus(int status, BString *description) {
	iNetwork->SetDescription( description );
	if( iNetwork->Session() ) {
		gg_change_status( iNetwork->Session(), status );
		iNetwork->SetStatus( status );
		BMessenger( this ).SendMessage( UPDATE_LIST );
	} else {
		iNetwork->Login( status, description );
	}
}

void MainWindow::UpdateBGDeskBar() {
	BMessage *message = new BMessage( BGDESKBAR_CHSTATE );
	message->AddInt16( "iStatus", iNetwork->GetStatus() );
	BMessenger( this ).SendMessage( message );
	delete message;
}

int MainWindow::SortUsers( const void *left, const void *right ) {
	const GaduListItem 	**firstItem( ( const GaduListItem ** ) left ),
						**secondItem( ( const GaduListItem ** ) right );
	BString users[ 2 ];

	users[ 0 ] = ( *firstItem )->iName->String();
	users[ 1 ] = ( *secondItem )->iName->String();
	return users[ 0 ].ICompare( users[ 1 ] );
}

void MainWindow::LoadIcons() {
	DEBUG_TRACE( "MainWindow::LoadIcons()\n" );
	iIconAvail	 			= GetBitmap( "online" );
	iIconBRB		 		= GetBitmap( "busy" );
	iIconInvisible	 		= GetBitmap( "invisible" );
	iIconNotAvail	 		= GetBitmap( "offline" );
	iIconAvailDescr 		= GetBitmap( "online_d" );
	iIconBRBDescr		 	= GetBitmap( "busy_d" );
	iIconInvisibleDescr 	= GetBitmap( "invisible_d" );
	iIconNotAvailDescr	 	= GetBitmap( "offline_d" );
}

void MainWindow::SetProfile( BString* aProfile ) {
	DEBUG_TRACE( "MainWindow::SetProfile()\n" );
	iProfile->Load( aProfile );
	SetTitle( iProfile->GetProfileName() );
	if( iProfile->GetUserlist()->IsValid() ) {
		iListItems = iProfile->GetUserlist()->GetList();
		BMessenger( this ).SendMessage( UPDATE_LIST );
	}

	if( iProfile->GetAutoStatus() != GG_STATUS_NOT_AVAIL ) {
		if( iNetwork->iSession )
			gg_change_status( iNetwork->iSession, iProfile->GetAutoStatus() );
		else
			iNetwork->Login();
	}
}

BBitmap *MainWindow::GetBitmap( const char* aName ) {
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

	if( bitmap->InitCheck() != B_OK ) {
		delete bitmap;
		return NULL;
	}

	return bitmap;
}

Profile* MainWindow::GetProfile() const {
	return iProfile;
}

Network* MainWindow::GetNetwork() const {
	return iNetwork;
}

BListView* MainWindow::ListView() const {
	return iListView;
}

GaduListItem* MainWindow::ListItem() const {
	return iListItem;
}

List* MainWindow::ListItems() const {
	return iListItems;
}

void MainWindow::SetMessenger( BMessenger& aMessenger ) {
	iDeskbarMessenger = aMessenger;
}
