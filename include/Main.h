
#ifndef __BEGADU_MAINWINDOW_H__
#define __BEGADU_MAINWINDOW_H__

#include <Window.h>
#include <Resources.h>

class BBitmap;
class BMenu;
class BMenuBar;
class BMenuField;
class BMenuItem;
class BListView;
class BPopUpMenu;
class BScrollView;
class BStringView;

class GaduListItem;
class GaduMenuItem;
class Network;
class Preferences;
class Profile;

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
		Profile* GetProfile() const;
		Network* GetNetwork() const;

	private:
		BBitmap* GetBitmap( const char* aName );
		void ChangeStatus(int status, BString *description);
		void UpdateBGDeskBar(void);
		void ClearContactList(void);

		/* to gg */
		Profile				*	iProfile;
		Network				*	iNetwork;

		/* to interface */
		BResources				iResources;
		BView				*	iGaduView;
		BScrollView			*	iScrollView;
		BListView			*	iListView;
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
		BStringView			*	iConnStatus;
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
