/*
 * ============================================================================
 *  Nazwa    : BeGadu z BeGadu.h
 *  Projekt  : BeGadu
 *  Authorzy : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Opis:
 *		Glowny modul
 *  Version  : 1.2
 * ============================================================================
 */

#ifndef __BEGADU_H__
#define __BEGADU_H__

#include <Application.h>

/* Forward classes */
class DebugWindow;
class MainWindow;
class ProfileSelector;

class BeGadu : public BApplication
	{
	public:
		BeGadu();
		virtual bool QuitRequested();
		virtual void MessageReceived(BMessage *message);
		virtual void ReadyToRun();
		void AddDeskbarIcon();
		void DelDeskbarIcon();
		MainWindow*	GetMainWindow() const;
		DebugWindow* GetDebugWindow() const;
		bool FirstRun();
		bool HideAtStart();
		BString* LastProfile();

	private:
		BMessenger			iMessenger;
		MainWindow  	*	iWindow;
		DebugWindow 	*	iDebugWindow;
		ProfileSelector	*	iProfileSelector;
		bool				iFirstRun;
		bool				iHideAtStart;
		BString			*	iLastProfile;
		bool				iReadyToRun;
};

#endif /* __BEGADU_H__ */
