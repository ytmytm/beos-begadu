
#ifndef __BEGADU_GADUMENUITEM_H__
#define __BEGADU_GADUMENUITEM_H__

#include <MenuItem.h>

class BBitmap;
class BString;

class GaduMenuItem : public BMenuItem
	{
	public:
		GaduMenuItem( BBitmap *aIcon = NULL, const char *aLabel = NULL, BMessage *aMessage = NULL );
		~GaduMenuItem();
		virtual void DrawContent();
		virtual void GetContentSize( float *aWidth, float *aHeight );
	private:
		BBitmap		*	iIcon;
		BString		*	iLabel;
	};

#endif // __BEGADU_GADUMENUITEM_H__
