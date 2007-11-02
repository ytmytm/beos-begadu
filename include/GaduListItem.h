
#ifndef __BEGADU_GADULISTITEM_H__
#define __BEGADU_GADULISTITEM_H__

#include <ListItem.h>
#include <libgadu.h>

class BBitmap;
class BMessage;
class BRect;
class BFont;
class BString;
class BResources;
class Person;

class GaduListItem : public BListItem
	{
	public:
		GaduListItem(Person *aPerson, int aStatus, const char *aDescription, BResources *aRes );
		~GaduListItem();
		virtual void DrawItem( BView* aOwner, BRect aFrame, bool aComplete = false );
		virtual void Update( BView* aOwner, const BFont* aFont );
		inline uin_t const UIN(void) { return iUIN; }
		inline Person *GetPerson(void) { return iPerson; }
		BString		*	iName;

	private:
		void SetIcon( int status );
		BBitmap	*GetBitmap( const char* aName );

		uin_t iUIN;
		Person *iPerson;

		BString		*	iDescription;
		int 			iStatus;

		BFont		*	iNameFont;
		BFont		*	iDescriptionFont;
		BBitmap		*	iIcon;
		BResources	*	iResources;
	};

#endif /* __BEGADU_GADULISTITEM_H__ */
