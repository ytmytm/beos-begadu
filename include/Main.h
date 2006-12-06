/*
 * ============================================================================
 *  Nazwa    : Main z Main.h
 *  Projekt  : BeGadu
 *  Authorzy : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Opis:
 *		Glowne okno programu
 *  Version  : 1.2
 * ============================================================================
 */

#ifndef __BEGADU_MAINWINDOW_H__
#define __BEGADU_MAINWINDOW_H__

#include <Window.h>
#include <Resources.h>

/* Forward references */
class Network;
class Preferences;
class List;
class BStringView;
class BMenuBar;
class BMenuItem;
class BMenu;
class BBitmap;
class BListItem;
class BListView;
class Profile;
class BPopUpMenu;
class BMenuField;
class GaduListItem;
class GaduMenuItem;
class BScrollView;
class Preferences;

class MainWindow : public BWindow
	{
	public:
		MainWindow( BString* aProfile );
		~MainWindow();
		virtual bool QuitRequested();
		virtual void MessageReceived( BMessage* aMessage );
		void SetStatus( char* aStatus );
		void SetProfile( BString* aProfile );
		void SetMessenger( BMessenger& aMessenger );
		static int SortUsers( const void*, const void* );
		void LoadIcons();
		BBitmap* GetBitmap( const char* aName );
		Profile* GetProfile() const;
		Network* GetNetwork() const;
		BListView* ListView() const;
		GaduListItem* ListItem() const;
		List* ListItems() const;

	private:
		/* to gg */
		Profile				*	iProfile;
		Network				*	iNetwork;

		/* to interface */
		BResources				iResources;
		BView				*	iGaduView;
		BScrollView			*	iScrollView;
		BListView			*	iListView;
		GaduListItem		*	iListItem;
		List				*	iListItems;
		BMenuBar			*	iMenuBar;
		BMenu				*	iMenu;
		BMenu				*	iProfilesMenu;
		BMenu				*	iUserlistMenu;
		BMenuItem			*	iProfileItem;

		BMenuItem			*	iListMenu;
		BMenuItem			*	iListImport;
		BMenuItem			*	iListExport;

		BMenuItem			*	iAddPerson;
		BMenuItem			*	iDelPerson;
		BMenuItem			*	iCatalog;
		BMenuItem			*	iAbout;
		BMenuItem			*	iPreferences;
		BView				*	iIconsView;

		BMenuField			*	iStatus;
		BPopUpMenu			*	iStatusMenu;
		GaduMenuItem		*	iAvail;
		GaduMenuItem		*	iBRB;
		GaduMenuItem		*	iInvisible;
		GaduMenuItem		*	iNotAvail;
		GaduMenuItem		*	iDescr;
		BBitmap				*	iIconAvail;
		BBitmap				*	iIconBRB;
		BBitmap				*	iIconInvisible;
		BBitmap				*	iIconNotAvail;
		BBitmap				*	iIconAvailDescr;
		BBitmap				*	iIconBRBDescr;
		BBitmap				*	iIconInvisibleDescr;
		BBitmap				*	iIconNotAvailDescr;
		BMessenger				iDeskbarMessenger;
		Preferences			*	iPreferencesWindow;
	};

#endif /* __BEGADU_MAINWINDOW_H__ */
