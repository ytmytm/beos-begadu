#include <stdio.h>
#include <stdlib.h>
#include <Message.h>
#include <ListItem.h>
#include <Bitmap.h> 
#include <Rect.h>
#include <Font.h>
#include <String.h>
#include <View.h>
#include <Roster.h>
#include <Resources.h>
#include <File.h>
#include <DataIO.h>
#include "GaduListItem.h"
#include "GfxStuff.h"
extern "C" {
#include "libgadu.h"
}

GaduListItem::GaduListItem( BString *aPerson, int aStatus, BString *aDescription, BResources *aRes ) : BListItem()
	{
	iResources = aRes;
	iStatus = aStatus;
	iIcon = NULL;
	SetIcon( iStatus );
	iName = aPerson;
	iDescription = aDescription;
	iNameFont = new BFont( be_plain_font );
	iNameFont->SetSize( 15.0 );
	iDescriptionFont = new BFont( be_plain_font );
	iDescriptionFont->SetSize( 10.0 );
	float woi = iNameFont->StringWidth( iName->String() );
	SetWidth( woi );
	}

GaduListItem::~GaduListItem()
	{
	if( iIcon )
		{
		delete iIcon;
		iIcon = NULL;
		}
	if( iNameFont )
		{
		delete iNameFont;
		iNameFont = NULL;
		}
	if( iDescriptionFont )
		{
		delete iDescriptionFont;
		iDescriptionFont = NULL;
		}
	}

void GaduListItem::SetIcon( int aStatus )
	{
	if( iIcon )
		{
		delete iIcon;
		iIcon = NULL;
		}
		
	switch( aStatus )
		{
		case GG_STATUS_NOT_AVAIL:
			{
			iIcon = GetBitmap( "offline" );
			break;
			}
			
		case GG_STATUS_NOT_AVAIL_DESCR:
			{
			iIcon = GetBitmap( "offline_d" );
			break;
			}
			
		case GG_STATUS_INVISIBLE:
			{
			iIcon = GetBitmap( "invisible" );
			break;
			}
		
		case GG_STATUS_INVISIBLE_DESCR:
			{
			iIcon = GetBitmap( "invisible_d" );
			break;
			}
		
		case GG_STATUS_BUSY:
			{
			iIcon = GetBitmap( "busy" );
			break;
			}
		
		case GG_STATUS_BUSY_DESCR:
			{
			iIcon = GetBitmap( "busy_d" );
			break;
			}
			
		case GG_STATUS_AVAIL:
			{
			iIcon = GetBitmap( "online" );
			break;
			}
			
		case GG_STATUS_AVAIL_DESCR:
			{
			iIcon = GetBitmap( "online_d" );
			break;
			}
		}
	}

void GaduListItem::DrawItem( BView* aOwner, BRect aFrame, bool aComplete )
	{
	rgb_color color;
	rgb_color color_name;
	rgb_color color_description;
	if( IsSelected() || aComplete )
		{
		if( IsSelected() )
			{
			color.red = color.green = color.blue = 100;
			}
		else
			{
			color = aOwner->ViewColor();
			}
//		aOwner->SetLowColor( color );
		aOwner->SetHighColor( color );
		aOwner->FillRect( aFrame );
		}
	else
		{
		color = aOwner->ViewColor();
//		aOwner->SetLowColor( color );
		aOwner->SetHighColor( color );
		aOwner->FillRect( aFrame );
		}

	if( iIcon )
		{
		aFrame.left += 2;
		aFrame.top += 8;
		aFrame.bottom = aFrame.top + 20;
		BRect r( aFrame.left,
				 aFrame.top,
				 aFrame.left + 18,
				 aFrame.top + 16 );
		aOwner->SetDrawingMode( B_OP_ALPHA );
		aOwner->SetLowColor( 255, 255, 255, 0 );
		aOwner->DrawBitmap( iIcon, r );
		}
	if(	IsSelected() )
		{
		color_name.red = color_name.blue = color_name.green = color_name.alpha = 255;
		color_description.red = color_description.blue = color_description.green = color_description.alpha = 175;
		}
	else
		{
		color_name.red = color_name.blue = color_name.green = color_name.alpha = 0;
		color_description.red = color_description.blue = color_description.green = color_description.alpha = 50;
		}
	aOwner->SetDrawingMode( B_OP_OVER );
//	aOwner->MovePenTo(frame.left + 21, (((frame.bottom+frame.top)/2.0))+4.0);
	aOwner->SetFont( iNameFont );
	aOwner->MovePenTo( aFrame.left + 21, aFrame.top + 9 );
	aOwner->SetHighColor( color_name );
	aOwner->DrawString( iName->String() );
	aOwner->SetFont( iDescriptionFont );
	aOwner->SetHighColor( color_description );
	aOwner->MovePenTo( aFrame.left + 21, aFrame.top + 21 );
	aOwner->DrawString( iDescription->String() );
	}

void GaduListItem::Update( BView* aOwner, const BFont* aFont )
	{
	SetHeight( 35.0 );
	}

BBitmap *GaduListItem::GetBitmap( const char* aName )
	{
	BBitmap 	*bitmap = NULL;
	size_t 		len = 0;
	status_t 	error;	

	const void *data = iResources->LoadResource( 'BBMP', aName, &len );

	BMemoryIO stream( data, len );
	
	BMessage archive;
	error = archive.Unflatten( &stream );
	if( error != B_OK )
		return NULL;
	bitmap = new BBitmap( &archive );
	if( !bitmap )
		return NULL;

	if( bitmap->InitCheck() != B_OK )
		{
		delete bitmap;
		return NULL;
		}
	
	return bitmap;
	}
