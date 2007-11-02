
#include <Bitmap.h>
#include <Box.h>
#include <Button.h>
#include <RadioButton.h>
#include <TextView.h>
#include <Screen.h>
#include <String.h>

#include <libgadu.h>

#include "Msg.h"
#include "Description.h"
#include "Main.h"
#include "Network.h"
#include "GfxStuff.h"
#include "globals.h"

#define DESCRIPTION_NAME "Change description"

IconsView::IconsView( BRect aRect, BResources *aResources )
			   : BView( aRect, "IconsView", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE )
	{
	iResources = aResources;
	if( iResources ) {
		iIconAvail = GetBitmap( "online_d" );
		iIconBusy = GetBitmap( "busy_d" );
		iIconInvisible = GetBitmap( "invisible_d" );
		iIconNotAvail = GetBitmap( "offline_d" );
	}
}

IconsView::~IconsView() {
	delete iIconAvail;
	delete iIconBusy;
	delete iIconInvisible;
	delete iIconNotAvail;
}

void IconsView::Draw( BRect aRect ) {
	if( ( iIconAvail ) &&
		( iIconBusy ) &&
		( iIconInvisible ) &&
		( iIconNotAvail ) ) {
		if( Window()->Lock() ) {
			SetDrawingMode( B_OP_ALPHA );
			SetLowColor( 255, 255, 255, 0 );
			DrawBitmap( iIconAvail, BPoint( 5, 5 ) );
			DrawBitmap( iIconBusy, BPoint( 5, 25 ) );
			DrawBitmap( iIconInvisible, BPoint( 5, 45 ) );
			DrawBitmap( iIconNotAvail, BPoint( 5, 65 ) );
			Window()->Unlock();
		}
	}
}

void IconsView::AttachedToWindow() {
	if( Parent() )
		SetViewColor( Parent()->ViewColor() );
	else
		SetViewColor( 255, 255, 255 );
}

BBitmap* IconsView::GetBitmap( const char* aName ) {
	BBitmap 	*bitmap = NULL;
	size_t 		len = 0;
	status_t 	error;	

	const void *data = iResources->LoadResource( 'BBMP', aName, &len );

	BMemoryIO stream(data, len);

	BMessage archive;
	error = archive.Unflatten(&stream);
	if (error != B_OK)
		return NULL;
	bitmap = new BBitmap(&archive);
	if(!bitmap)
		return NULL;

	if(bitmap->InitCheck() != B_OK) {
		delete bitmap;
		return NULL;
	}

	return bitmap;
}

Description::Description( MainWindow* aWindow, BRect aRect, BResources* aRes )
	: BWindow( aRect,
			   "",
			   B_FLOATING_WINDOW,
			   B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_ASYNCHRONOUS_CONTROLS )
	{
	BScreen *screen = new BScreen( this );
	display_mode mode;
	screen->GetMode( &mode );
	delete screen;

	int32 width = 250;
	int32 height = 140;
	int32 x_wind = mode.timing.h_display / 2 - ( width / 2);
	int32 y_wind = mode.timing.v_display / 2 - ( height / 2 );
	MoveTo(x_wind,y_wind);
	ResizeTo(width,height);

	SetTitle( _T( DESCRIPTION_NAME ) );
	iWindow = aWindow;
	iResources = aRes;

	BRect r = Bounds();
	r.left = 5;
	r.right = r.right - 5;
	r.top = 5;
	r.bottom = r.bottom - 40;
	iBox = new BBox( r, "Description box" );

	r = iBox->Bounds();
	r.left = 3;
	r.right = r.left + 25;
	r.top = 3;
	r.bottom = r.bottom - 3;
	iIconsView = new IconsView( r, iResources );
	iBox->AddChild( iIconsView );
	AddChild( iBox );

	r = iBox->Bounds();
	r.left = r.left + 30;
	r.right = r.left + 15;
	r.top = 7;
	r.bottom = r.top + 12;
	iAvail = new BRadioButton( r, "AvailDesc", "", new BMessage( SET_AVAIL ) );
	iBox->AddChild( iAvail );

	r = iBox->Bounds();
	r.left = r.left + 30;
	r.right = r.left + 15;
	r.top = 27;
	r.bottom = r.top + 12;
	iBusy = new BRadioButton( r, "BusyDesc", "", new BMessage( SET_BRB ) );
	iBox->AddChild( iBusy );

	r = iBox->Bounds();
	r.left = r.left + 30;
	r.right = r.left + 15;
	r.top = 47;
	r.bottom = r.top + 12;
	iInvisible = new BRadioButton( r, "InvisibleDesc", "", new BMessage( SET_INVIS ) );
	iBox->AddChild( iInvisible );

	r = iBox->Bounds();
	r.left = r.left + 30;
	r.right = r.left + 15;
	r.top = 67;
	r.bottom = r.top + 12;
	iNotAvail = new BRadioButton( r, "NotAvailDesc", "", new BMessage( SET_NOT_AVAIL ) );
	iBox->AddChild( iNotAvail );

	r = iBox->Bounds();
	r.left = r.left + 50;
	r.right = r.right - 3;
	r.top = r.top + 3;
	r.bottom = r.bottom - 3;
	BRect t = iBox->Bounds();
	t.right = t.right - 60;
	iDescription = new BTextView( r, "DescriptionTextView", t, B_FOLLOW_ALL, B_WILL_DRAW );
	iDescription->SetMaxBytes( 160 );
	iBox->AddChild( iDescription );

	r = Bounds();
	r.left = r.right - 140;
	r.right = r.right - 75;
	r.top = r.bottom - 30;
	r.bottom = -5;
	BButton *button;
    button = new BButton( r, "change button", _T("Change"), new BMessage( DESCRIPTION_OK ) );
    button->MakeDefault( true );
    AddChild( button );

    r = Bounds();
	r.left = r.right - 65;
	r.right = r.right -5;
	r.top = r.bottom - 30;
	r.bottom = -5;

    button = new BButton( r, "cancel button", _T("Cancel"), new BMessage( DESCRIPTION_CANCEL ) );
    AddChild( button );

	/* setting current settings */
	int status = iWindow->GetNetwork()->GetStatus();
	if( status == GG_STATUS_AVAIL || status == GG_STATUS_AVAIL_DESCR )
		{
		iStatus = GG_STATUS_AVAIL_DESCR;
		iAvail->SetValue( B_CONTROL_ON );
		}
	else if( status == GG_STATUS_BUSY || status == GG_STATUS_BUSY_DESCR )
		{
		iStatus = GG_STATUS_BUSY_DESCR;
		iBusy->SetValue( B_CONTROL_ON );
		}
	else if( status == GG_STATUS_INVISIBLE || status == GG_STATUS_INVISIBLE_DESCR )
		{
		iStatus = GG_STATUS_INVISIBLE_DESCR;
		iInvisible->SetValue( B_CONTROL_ON );
		}
	else if( status == GG_STATUS_NOT_AVAIL || status == GG_STATUS_NOT_AVAIL_DESCR )
		{
		iStatus = GG_STATUS_NOT_AVAIL_DESCR;
		iNotAvail->SetValue( B_CONTROL_ON );
		}
	BString *description = fromISO2(iWindow->GetNetwork()->iDescription->String());
	iDescription->SetText( description->String(), description->Length() );
	delete description;
}

void Description::MessageReceived( BMessage* aMessage ) {
	switch( aMessage->what ) {
		case SET_AVAIL:
			iStatus = GG_STATUS_AVAIL_DESCR;
			break;
		case SET_BRB:
			iStatus = GG_STATUS_BUSY_DESCR;
			break;
		case SET_INVIS:
			iStatus = GG_STATUS_INVISIBLE_DESCR;
			break;
		case SET_NOT_AVAIL:
			iStatus = GG_STATUS_NOT_AVAIL_DESCR;
			break;
		case DESCRIPTION_OK:
			{
			BString description;
			BMessage *desc = new BMessage( SET_DESCRIPTION );
			fprintf( stderr, "iStatus = %d\n", iStatus );
			desc->AddInt8( "iStatus", iStatus );
			if( iDescription->LockLooper() ) {
				description.SetTo( iDescription->Text() );
				iDescription->UnlockLooper();
			}
			BString *desciso = toISO2(description.String());
			description.SetTo(desciso->String());
			desc->AddString( "iDescription", description );
			BMessenger( iWindow ).SendMessage( desc );
			delete desc;
			delete desciso;
			BMessenger( this ).SendMessage( B_QUIT_REQUESTED );
			break;
			}
		case DESCRIPTION_CANCEL:
			BMessenger( this ).SendMessage( B_QUIT_REQUESTED );
			break;
		default:
			BWindow::MessageReceived( aMessage );
	}
}

void Description::Show() {
    BWindow::Show();
}
