/*
 * ============================================================================
 *  Nazwa    : GaduMenuItem z GaduMenuItem.h
 *  Projekt  : BeGadu
 *  Authorzy : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Opis:
 *		My version of class BMenuItem
 *  Version  : 1.2
 * ============================================================================
 */


#ifndef __BEGADU_GADUMENUITEM_H__
#define __BEGADU_GADUMENUITEM_H__

#include <MenuItem.h>

/* forward classes */
class BBitmap;
class BFont;
class BRect;
class BString;
class BMessage;

class GaduMenuItem : public BMenuItem
	{
	public:
		GaduMenuItem( BBitmap *aIcon, const char *aLabel, BMessage *aMessage );
		~GaduMenuItem();
		virtual void DrawContent();
		virtual void GetContentSize( float *aWidth, float *aHeight );
		BBitmap		*	iIcon;
		BString		*	iLabel;
	};

#endif // __BEGADU_GADUMENUITEM_H__
