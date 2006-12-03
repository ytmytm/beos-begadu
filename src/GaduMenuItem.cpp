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

#include <stdio.h>
#include <stdlib.h>
#include <Message.h>
#include <MenuItem.h>
#include <Bitmap.h> 
#include <Rect.h>
#include <Font.h>
#include <String.h>
#include <View.h>
#include "GaduMenuItem.h"
#include "GfxStuff.h"

GaduMenuItem::GaduMenuItem(  BBitmap *aIcon, const char *aLabel, BMessage *aMessage ) : BMenuItem( aLabel, aMessage )
{
	iIcon = NULL;
	if( aIcon )
		iIcon = aIcon;
	iLabel = new BString( aLabel );
}

GaduMenuItem::~GaduMenuItem()
{
	//delete iIcon;
	delete iLabel;
}

void GaduMenuItem::DrawContent()
{
	rgb_color color;
	BRect frame = Frame();
	frame.left += 2;
	frame.top += 2;
	frame.bottom = frame.top + 20;
	BRect r( frame.left,
			 frame.top,
			 frame.left + 18,
			 frame.top + 16);
	Menu()->SetDrawingMode(B_OP_ALPHA);
	Menu()->SetLowColor(0, 0, 0, 255);

	if(iIcon)
		Menu()->DrawBitmap( iIcon, r );

	frame.left += 2;
	Menu()->SetDrawingMode(B_OP_OVER);
	Menu()->MovePenTo(frame.left + 21, (((frame.bottom+frame.top)/2.0))+4.0);
	Menu()->DrawString( iLabel->String());
}

void GaduMenuItem::GetContentSize( float *aWidth, float *aHeight )
{
	BMenuItem::GetContentSize( aWidth, aHeight );
	*aHeight = 20;
}
