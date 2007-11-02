
#ifndef __BEGADU_GADULISTITEM_H__
#define __BEGADU_GADULISTITEM_H__

#include <ListItem.h>

class BBitmap;
class BMessage;
class BRect;
class BFont;
class BString;
class BResources;

class GaduListItem : public BListItem
	{
	public:
		GaduListItem( const char* aPerson, int aStatus, const char* aDescription, BResources* aRes );
		~GaduListItem();
		virtual void DrawItem( BView* aOwner, BRect aFrame, bool aComplete = false );
		virtual void Update( BView* aOwner, const BFont* aFont );
		BString		*	iName;
		BString		*	iDescription;
		int 			iStatus;
	private:
		void SetIcon( int status );
		BBitmap	*GetBitmap( const char* aName );
		BFont		*	iNameFont;
		BFont		*	iDescriptionFont;
		BBitmap		*	iIcon;
		BResources	*	iResources;
	};

#endif /* __BEGADU_GADULISTITEM_H__ */
