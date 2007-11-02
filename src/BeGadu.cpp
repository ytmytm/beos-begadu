
#include <Deskbar.h>
#include <Roster.h>
#include <StorageKit.h>
#include <Screen.h>
#include <String.h>

#include "Msg.h"
#include "BeGadu.h"
#include "Network.h"
#include "Main.h"
#include "Person.h"
#include "Preferences.h"
#include "ProfileWizard.h"

#include "globals.h"

BeGadu::BeGadu() : BApplication( APP_MIME ) {
	/* we're checking configuration */
	iFirstRun = false;
	iHideAtStart = false;
	iLastProfile = new BString( "" );
	iWindow = NULL;
//	iProfileSelector = NULL;
	iReadyToRun = false;
	BPath path;
	BEntry entry;
	status_t error;

	find_directory( B_USER_SETTINGS_DIRECTORY, &path );
	BDirectory * bg_conf = new BDirectory( path.Path() );

	if( bg_conf->FindEntry( "BeGadu", &entry ) != B_OK ) {
		path.Append( "BeGadu" );
		bg_conf->CreateDirectory( path.Path(), bg_conf );
	}

	if( bg_conf->FindEntry( "Profiles", &entry ) != B_OK ) {
		find_directory( B_USER_SETTINGS_DIRECTORY, &path );
		path.Append( "BeGadu/Profiles" );
		error = bg_conf->CreateDirectory( path.Path(), bg_conf );
		iFirstRun = true;
	}

	find_directory( B_USER_SETTINGS_DIRECTORY, &path );
	path.Append( "BeGadu/Profiles" );
	bg_conf->SetTo( path.Path() );

	iFirstRun = (bg_conf->CountEntries() == 0);

	/* loading configuration */
	find_directory( B_USER_SETTINGS_DIRECTORY, &path );
	path.Append( "BeGadu/Config" );
	BFile file( path.Path(), B_READ_ONLY );
	fprintf( stderr, _T("Loading configuration from %s\n"), path.Path() );
	BMessage *cfgmesg = new BMessage();
	if( file.InitCheck() == B_OK ) {
		cfgmesg->Unflatten( &file );
		file.Unset();
	}
	if( cfgmesg->FindBool( "iFirstRun", &iFirstRun ) != B_OK ) {
		iFirstRun =  true;
	}
	if( cfgmesg->FindString( "iLastProfile", iLastProfile ) != B_OK ) {
		iLastProfile->SetTo( "" );
	}
	
	delete cfgmesg;
	delete bg_conf;

	if( iFirstRun ) {
		fprintf( stderr, _T("First run, starting Profile Wizard...\n") );
		BMessenger( this ).SendMessage( new BMessage( OPEN_PROFILE_WIZARD ) );
	} else {
		fprintf( stderr, _T("Configuration OK.\n") );
		BMessenger( this ).SendMessage( new BMessage( CONFIG_OK ) );
	}
}

void BeGadu::MessageReceived( BMessage *aMessage ) {
	switch( aMessage->what ) {
		/* sending mesgs from libgadu to network */
		case GOT_MESSAGE:
		case ADD_HANDLER:
		case DEL_HANDLER:
			BMessenger( iWindow->GetNetwork() ).SendMessage( aMessage );
			break;
		case ADD_MESSENGER:
			DEBUG_TRACE( "BeGadu::MessageReceived( ADD_MESSENGER )\n" );
			aMessage->FindMessenger( "messenger", &iMessenger );
			if( iWindow ) {
				iWindow->SetMessenger( iMessenger );
				BMessenger( iMessenger ).SendMessage( PROFILE_SELECTED );
			}
			break;
		case SET_AVAIL:
		case SET_BRB:
		case SET_INVIS:
		case SET_NOT_AVAIL:
		case SET_DESCRIPTION:
		case BEGG_ABOUT:
		case SHOW_MAIN_WINDOW:
		case CHANGE_DESCRIPTION:
		case PREFERENCES_SWITCH:
			if( iWindow )
				BMessenger( iWindow ).SendMessage( aMessage );
			break;
		case OPEN_PROFILE_WIZARD:
			{
			DEBUG_TRACE( "BeGadu::MessageReceived( OPEN_PROFILE_WIZARD )\n" );
//			if( iProfileSelector )
//				iProfileSelector = NULL;
			if( iWindow ) {
				BMessenger( iWindow ).SendMessage( new BMessage( CLOSE_MAIN_WINDOW ) );
				if( iWindow->Lock() )
					iWindow->Quit();
				iWindow = NULL;
			}
			ProfileWizard *pw = new ProfileWizard();
			pw->Show();
			break;
			}
		case CONFIG_OK:
			{
			DEBUG_TRACE( "BeGadu::MessageReceived( CONFIG_OK )\n" );
			iReadyToRun = true;
			AddDeskbarIcon();
			Profile *profile = new Profile();
			int ret = profile->Load( iLastProfile );
			if( ret != 0 ) {
				delete profile;
				BMessenger( be_app ).SendMessage( new BMessage( PROFILE_SELECT ) );
				break;
			}
			if( strcmp( profile->GetProfilePassword(), "" ) != 0 ) {
				BResources res;
				BRoster roster;
				entry_ref ref;
				BFile resfile;
				roster.FindApp( APP_MIME, &ref );
				resfile.SetTo( &ref, B_READ_ONLY );
				res.SetTo( &resfile );
				BScreen *screen = new BScreen( B_MAIN_SCREEN_ID );
				display_mode mode;
				screen->GetMode( &mode );
//				int32 width = 250;
//				int32 height = 110; // 70
//				int32 x_wind = mode.timing.h_display / 2 - ( width / 2);
//				int32 y_wind = mode.timing.v_display / 2 - ( height / 2 );
//				int32 new_width = x_wind + width;	// x 2
//				int32 new_height = y_wind + height;		// x 2
				BMessenger( iMessenger ).SendMessage( new BMessage( PROFILE_NOT_SELECTED ) );
//				iProfileSelector = new ProfileSelector( iLastProfile, BRect( x_wind, y_wind, new_width, new_height ), &res );
//				if( iProfileSelector->LockLooper() ) {
//					iProfileSelector->Show();
//					iProfileSelector->UnlockLooper();
//				}
			} else {
				BMessenger( iMessenger ).SendMessage( new BMessage( PROFILE_SELECTED ) );
				iWindow = new MainWindow( iLastProfile );
				if( !iHideAtStart ) {
					if( iWindow->LockLooper() ) {
						iWindow->Show();
						iWindow->UnlockLooper();
					}
				} else {
					if( iWindow->LockLooper() ) {
						iWindow->Show();
						iWindow->Hide();
						iWindow->UnlockLooper();
					}
				}
			}
			break;
			}
		case PROFILE_CREATED:
			DEBUG_TRACE( "BeGadu::MessageReceived( PROFILE_CREATED )\n" );
			iReadyToRun = true;
			AddDeskbarIcon();
			aMessage->FindString( "ProfileName", iLastProfile );
			fprintf( stderr, _T("Setting last profile to %s\n"), iLastProfile->String() );
			iFirstRun = false;
			BMessenger( iMessenger ).SendMessage( new BMessage( PROFILE_SELECTED ) );
			iWindow = new MainWindow( iLastProfile );
			if( iWindow->LockLooper() ) {
				if( iWindow->IsHidden() )
					iWindow->Show();
				else
					iWindow->Activate();
				iWindow->UnlockLooper();
			}
			break;
		case PROFILE_SELECT:
			DEBUG_TRACE( "BeGadu::MessageReceived( PROFILE_SELECT )\n" );
//			if( iProfileSelector )
//				iProfileSelector->Activate();
//			else
				{
				BResources res;
				BRoster roster;
				entry_ref ref;
				BFile resfile;
				roster.FindApp( APP_MIME, &ref );
				resfile.SetTo( &ref, B_READ_ONLY );
				res.SetTo( &resfile );
				BScreen *screen = new BScreen( B_MAIN_SCREEN_ID );
				display_mode mode;
				screen->GetMode( &mode );
//				int32 width = 250;
//				int32 height = 110; // 70
//				int32 x_wind = mode.timing.h_display / 2 - ( width / 2);
//				int32 y_wind = mode.timing.v_display / 2 - ( height / 2 );
//				int32 new_width = x_wind + width;	// x 2
//				int32 new_height = y_wind + height;		// x 2
				BMessenger( iMessenger ).SendMessage( new BMessage( PROFILE_NOT_SELECTED ) );
//				iProfileSelector = new ProfileSelector( iLastProfile, BRect( x_wind, y_wind, new_width, new_height ), &res );
//				if( iProfileSelector->LockLooper() ) {
//					iProfileSelector->Show();
//					iProfileSelector->UnlockLooper();
//				}
			}
			break;
		case PROFILE_SWITCH:
			{
			DEBUG_TRACE( "BeGadu::MessageReceived( PROFILE_SWITCH )\n" );
			if( iWindow ) {
				BMessenger( iWindow ).SendMessage( new BMessage( CLOSE_MAIN_WINDOW ) );
				if( iWindow->Lock() )
					iWindow->Quit();
				iWindow = NULL;
			}
			Profile* profile = new Profile();
			BString* name = new BString( "" );
			aMessage->FindString( "iProfileName", name );
			int ret = profile->Load( name );
			if( ret != 0 ) {
				delete profile;
				BMessenger( this ).SendMessage( new BMessage( PROFILE_SELECT ) );
				break;
			}
			// XXX loaded profile password empty?
			if( strcmp( profile->GetProfilePassword(), "" ) == 0 ) {
				BMessenger( iMessenger ).SendMessage( new BMessage( PROFILE_SELECTED ) );
				iWindow = new MainWindow( iLastProfile );
//				iWindow = new MainWindow( name );
				if( iWindow->LockLooper() ) {
					iWindow->Show();
					iWindow->UnlockLooper();
				}
			} else {
			// XXX what's that for?
				BResources res;
				BRoster roster;
				entry_ref ref;
				BFile resfile;
				roster.FindApp( APP_MIME, &ref );
				resfile.SetTo( &ref, B_READ_ONLY );
				res.SetTo( &resfile );
				BScreen *screen = new BScreen( B_MAIN_SCREEN_ID );
				display_mode mode;
				screen->GetMode( &mode );
//				int32 width = 250;
//				int32 height = 110; // 70
//				int32 x_wind = mode.timing.h_display / 2 - ( width / 2);
//				int32 y_wind = mode.timing.v_display / 2 - ( height / 2 );
//				int32 new_width = x_wind + width;	// x 2
//				int32 new_height = y_wind + height;		// x 2
				BMessenger( iMessenger ).SendMessage( new BMessage( PROFILE_NOT_SELECTED ) );
//				iProfileSelector = new ProfileSelector( name, BRect( x_wind, y_wind, new_width, new_height ), &res );
//				if( iProfileSelector->LockLooper() ) {
//					iProfileSelector->Show();
//					iProfileSelector->UnlockLooper();
//				}
			}
			break;
			}
		case PROFILE_SELECTED:
			{
			DEBUG_TRACE( "BeGadu::MessageReceived( PROFILE_SELECTED )\n" );
//			if( iProfileSelector )
//				iProfileSelector = NULL;
			BString *profile = new BString( "" );
			aMessage->FindString( "iProfileName", profile );
			BMessenger( iMessenger ).SendMessage( new BMessage( PROFILE_SELECTED ) );
			iWindow = new MainWindow( profile );
			if( !iHideAtStart ) {
				if( iWindow->LockLooper() ) {
					iWindow->Show();
					iWindow->UnlockLooper();
				}
			} else {
				if( iWindow->LockLooper() ) {
					iWindow->Show();
					iWindow->Hide();
					iWindow->UnlockLooper();
				}
			}
			break;
			}
		case PROFILE_NOT_SELECTED:
			DEBUG_TRACE( "BeGadu::MessageReceived( PROFILE_NOT_SELECTED )\n" );
//			if( iProfileSelector )
//				iProfileSelector = NULL;
			BMessenger( iMessenger ).SendMessage( new BMessage( PROFILE_NOT_SELECTED ) );
			break;
		case BEGG_QUIT:
			DEBUG_TRACE( "BeGadu::MessageReceived( BEGG_QUIT )\n" );
			if( iWindow )
				BMessenger( iWindow ).SendMessage( aMessage );
			else
				BMessenger( be_app ).SendMessage( B_QUIT_REQUESTED );
			break;
		default:
			BApplication::MessageReceived( aMessage );
			break;
		}
}

void BeGadu::ReadyToRun() {
	DEBUG_TRACE( "BeGadu::ReadyToRun()\n" );
	if( iReadyToRun )
		AddDeskbarIcon();
}

bool BeGadu::QuitRequested() {
	DelDeskbarIcon();
//	if( iProfileSelector )
//		if( iProfileSelector->Lock() )
//			iProfileSelector->Quit();
	if( iWindow ) {
		if( iLastProfile )
			iLastProfile->SetTo( iWindow->GetProfile()->GetProfileName() );
		else
			iLastProfile = new BString( iWindow->GetProfile()->GetProfileName() );
		fprintf( stderr, _T("Last profile %s\n"), iLastProfile->String() );
		if( iWindow->Lock() )
			iWindow->Quit();
	}

	/* saving configuration */
	BMessage * cfgmesg = new BMessage();
	cfgmesg->AddBool( "iFirstRun", iFirstRun );
	cfgmesg->AddBool( "iHideAtStart", iHideAtStart );
	cfgmesg->AddString( "iLastProfile", *iLastProfile );

	BPath path;
	find_directory( B_USER_SETTINGS_DIRECTORY, &path );
	path.Append( "BeGadu/Config" );
	fprintf( stderr, _T("Saving configuration to %s\n"), path.Path() );
	BFile file( path.Path(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE );
	if( file.InitCheck() == B_OK ) {
		cfgmesg->Flatten( &file );
		file.Unset();
	}

	delete cfgmesg;
	BApplication::QuitRequested();
	return true;
}

void BeGadu::AddDeskbarIcon() {
	DEBUG_TRACE( "BeGadu::AddDeskbarIcon()\n" );
	BDeskbar deskbar;
	if( !deskbar.HasItem( "BGDeskbar" ) ) {
		BRoster roster;
		entry_ref ref;
		status_t status = roster.FindApp( APP_MIME, &ref );
		if( status != B_OK ) {
			fprintf( stderr, _T("Can't find BeGadu running: %s\n"), strerror( status ) );
			return;
		}
		status = deskbar.AddItem( &ref );
		if( status != B_OK ) {
			fprintf( stderr, _T("Can't put BeGadu into Deskbar: %s\n"), strerror( status ) );
			return;
		}
	}
}

void BeGadu::DelDeskbarIcon() {
	DEBUG_TRACE( "BeGadu::DelDeskbarIcon()\n" );
	BDeskbar deskbar;
	if( deskbar.HasItem( "BGDeskbar" ) )
		deskbar.RemoveItem( "BGDeskbar" );
}

MainWindow* BeGadu::GetMainWindow() const {
	return iWindow;
}
	
DebugWindow* BeGadu::GetDebugWindow() const {
	return iDebugWindow;
}

bool BeGadu::FirstRun() {
	return iFirstRun;
}

bool BeGadu::HideAtStart() {
	return iHideAtStart;
}

BString* BeGadu::LastProfile() {
	return iLastProfile;
}

int main() {
	BeGadu	*bgg = new BeGadu();
	bgg->Run();
	delete bgg;
	return 0;
}
