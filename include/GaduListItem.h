/*
 * ============================================================================
 *  Nazwa    : GaduListItem z GaduListItem.h
 *  Projekt  : BeGadu
 *  Authorzy : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Opis:
 *		Moja implementacja BListItem
 *  Version  : 1.2
 * ============================================================================
 */


#ifndef __BEGADU_GADULISTITEM_H__
#define __BEGADU_GADULISTITEM_H__

#include <ListItem.h>
#include <Resources.h>

/* Forward references */
class BBitmap;
class BMessage;
class BRect;
class BFont;
class BString;

class GaduListItem : public BListItem
	{
	public:
		GaduListItem( const char* aPerson, int aStatus, const char* aDescription, BResources* aRes );
		~GaduListItem();
		void SetIcon( int status );
		virtual void DrawItem( BView* aOwner, BRect aFrame, bool aComplete = false );
		virtual void Update( BView* aOwner, const BFont* aFont );
		BBitmap	*GetBitmap( const char* aName );
		BString		*	iName;
		BString		*	iDescription;
		BFont		*	iNameFont;
		BFont		*	iDescriptionFont;
		int 			iStatus;
		BBitmap		*	iIcon;
		BResources	*	iResources;
	};

#endif /* __BEGADU_GADULISTITEM_H__ */
