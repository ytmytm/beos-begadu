
#include <Bitmap.h>
#include <BitmapStream.h>
#include <Message.h>
#include <Resources.h>
#include <String.h>
#include <TranslatorRoster.h>
#include <TranslationUtils.h>
#include <Window.h>
#include <UTF8.h>
#include <stdio.h>

#include "GfxStuff.h"
#include "globals.h"

IconControl::IconControl(BRect frame, const char *name, BBitmap *MOut, BBitmap *MOver, BMessage *message) : BView(frame, name, B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE | B_ASYNCHRONOUS_CONTROLS) {
	IconOut = MOut;
	IconOver = MOver;
	IsMouseOver = false;
	IsDown = false;
	mesg = message;
	SetEventMask(B_POINTER_EVENTS, B_NO_POINTER_HISTORY | B_LOCK_WINDOW_FOCUS);
}

IconControl::~IconControl() {
	delete IconOut;
	delete IconOver;
}

void IconControl::AttachedToWindow() {
	if( Parent() )
		SetViewColor( Parent()->ViewColor() );
	else
		SetViewColor(100, 100, 100);
}

void IconControl::Draw(BRect updateRect) {
	SetDrawingMode(B_OP_ALPHA);
	SetLowColor(255,255,255,0);
	SetViewColor(B_TRANSPARENT_32_BIT);
	if(Window()->Lock()) {
		if(IsMouseOver)
			DrawBitmap(IconOver);
		else
			DrawBitmap(IconOut);
		Window()->Unlock();
	}
}

void IconControl::MouseMoved(BPoint point, uint32 transit, const BMessage *message) {
	BRect r = Bounds();
	IsMouseOver = (point.x >= r.left && point.x <= r.right && point.y >= r.top && point.y <= r.bottom);
	Draw(r);
}

void IconControl::MouseDown(BPoint point) {
	uint32 a;
	GetMouse(&point, &a, true);
	BRect r = Bounds();
	IsDown = (point.x >= r.left && point.x <= r.right && point.y >= r.top && point.y <= r.bottom);
}

void IconControl::MouseUp(BPoint point) {
	uint32 a;
	GetMouse(&point, &a, true);
	BRect r = Bounds();
	if (IsDown)
	{
		IsDown = !(point.x >= r.left && point.x <= r.right && point.y >= r.top && point.y <= r.bottom);
	}
}

void IconControl::MessageReceived( BMessage *message ) {
	switch( message->what ) {
		default:
			BView::MessageReceived(message);
			break;
	}
}

BBitmap* LoadBMP(char* filename) {
	BFile file(filename, B_READ_ONLY);
	BTranslatorRoster *roster = BTranslatorRoster::Default();
	BBitmapStream stream;
	BBitmap *result = NULL;
	if (roster->Translate(&file, NULL, NULL, &stream, B_TRANSLATOR_BITMAP) < B_OK)
		return NULL;
	stream.DetachBitmap(&result);
	return result;
}

BBitmap *LoadGFX(const char *filename) {
	return BTranslationUtils::GetBitmap(filename);
}

BitmapView::BitmapView( BRect frame, const char *name, BResources *res )
			   : BView( frame, "bitmap_view", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE )
{
	fResources = res;
	SetViewColor( 255, 255, 255 );
	fBitmap = GetBitmap(name);
	if( fBitmap ) {
		DEBUG_TRACE(" bitmap ok\n");
		bmbounds = fBitmap->Bounds();
	}
}

BitmapView::~BitmapView() {
	delete fBitmap;
}


void BitmapView::AttachedToWindow() {
	if( Parent() )
		SetViewColor( Parent()->ViewColor() );
	else
		SetViewColor( 255, 255, 255 );
}

void BitmapView::Draw( BRect updateRect ) {
	// calculate the offsets to center the bitmap
	float LOffset = (Bounds().Width() - bmbounds.Width()) / 2;
	float TOffset = (Bounds().Height() - bmbounds.Height()) / 2;	
	// Make a new rect w/ those coordinates
	BRect drawRect( LOffset, TOffset, Bounds().Width() - LOffset, Bounds().Height() - TOffset );
	if( fBitmap ) {
		if ( Window()->Lock() ) {
			DrawBitmap( fBitmap, drawRect );	
			Window()->Unlock();
		}
	}
}

BBitmap *BitmapView::GetBitmap(const char *name) {
	BBitmap 	*bitmap = NULL;
	size_t 		len = 0;
	status_t 	error;	

	const void *data = fResources->LoadResource('BBMP', name, &len);

	BMemoryIO stream(data, len);

	BMessage archive;
	error = archive.Unflatten(&stream);
//printf("bitmap: [%s], ok=%i\n", name, error==B_OK);
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

////////////////////

BString *fromISO2(const char *input) {
	// konwersja na utf8
	char *dstBuf = new char[strlen(input)*2];
	int32 state;
	int32 inLen = strlen(input);
	int32 outLen = inLen*2;
	convert_to_utf8(B_ISO2_CONVERSION,input,&inLen,dstBuf,&outLen,&state);
	dstBuf[outLen]=0;
	// prawie dobrze...
	BString *result = new BString(dstBuf);
	result->ReplaceAll("š","ą");
	result->ReplaceAll("Ľ","Ą");
	result->ReplaceAll("","ś");
	result->ReplaceAll("","Ś");
	result->ReplaceAll("","ź");
	result->ReplaceAll("","Ź");
	delete dstBuf;
	return result;
}

BString *toISO2(const char *input) {
	char *dstBuf = new char[strlen(input)+1];
	int32 state;
	int32 inLen = strlen(input);
	int32 outLen = inLen;
	convert_from_utf8(B_ISO2_CONVERSION,input,&inLen,dstBuf,&outLen,&state);
	dstBuf[outLen]=0;
	BString *result = new BString(dstBuf);
	delete dstBuf;
	return result;
}
