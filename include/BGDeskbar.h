/*
 * ============================================================================
 *  Nazwa    : BGDeskbar from BGDeskbar.h
 *  Projekt  : BeGadu
 *  Authorzy : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Opis:
 *		BGDeskbar replicant
 *  Version  : 1.2
 * ============================================================================
 */

#ifndef __BEGADU_DESKBAR_H__
#define __BEGADU_DESKBAR_H__

#include <View.h>
#include <Resources.h>

/* Forward references */
class BBitmap;
class BPopUpMenu;
class BMessage;
//class ProfileWizard;

class _EXPORT BGDeskbar : public BView
	{
	public:
		BGDeskbar();
		BGDeskbar( BMessage *aMessage );
		virtual ~BGDeskbar();

		virtual	status_t Archive( BMessage *aMessage, bool aDeep = true ) const;
		static BArchivable *Instantiate( BMessage *aData );
		virtual	void AttachedToWindow();
		virtual void DetachedFromWindow();
		virtual void Draw( BRect aRect );
		virtual void Pulse();
		virtual void MessageReceived( BMessage *aMessage );
		virtual void MouseDown( BPoint aWhere );
		void Initialize();
		void Remove();
		BBitmap	*GetBitmap( const char *name );
		
	private:
		BPopUpMenu		*	iMenu;
		BBitmap			*	iIcon;
		BBitmap			*	iIconAvail;
		BBitmap			*	iIconBusy;
		BBitmap			*	iIconInvisible;
		BBitmap			*	iIconNotAvail;
		BBitmap			*	iIconAvailDescr;
		BBitmap			*	iIconBusyDescr;
		BBitmap			*	iIconInvisibleDescr;
		BBitmap			*	iIconNotAvailDescr;
		BBitmap			*	iIconNewMessage;
		BBitmap			*	iIconQuit;
		time_t				iMesgAnimate;
		BResources			iResources;
	};

#endif /* __BEGADU_DESKBAR_H__ */
