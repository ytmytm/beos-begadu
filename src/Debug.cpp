/*
	Debug.cpp
	Code: aljen <aljen@gumisie.org>
	Homepage: http://gadu.beos.pl
*/

#include <Application.h>
#include "Debug.h"

#define DEBUGWINDOW_RECT BRect(150,50,500,350)
#define DEBUGWINDOW_NAME "Debug output"
void DebugWindow::AddLog(char *entry)
{
	dview->Insert(entry);
}
DebugWindow::DebugWindow() : BWindow(DEBUGWINDOW_RECT,DEBUGWINDOW_NAME, B_FLOATING_WINDOW, B_NOT_CLOSABLE | B_NOT_ZOOMABLE)
{
	rgb_color kolor = {0,255,0,0};
	BRect drect(Bounds());
	dview = new BTextView(drect,"debug",Bounds(),B_FOLLOW_ALL_SIDES,B_FULL_UPDATE_ON_RESIZE | B_WILL_DRAW | B_NAVIGABLE);
	BFont *font = new BFont(be_plain_font);
	font->SetSize(18.0);
	dview->SetFontAndColor(font,B_FONT_ALL, &kolor);
	dview->MakeEditable(false);
	dview->SetViewColor(0,0,0);
	dview->Insert("Debug window created, starting logging :)");
	AddChild(dview);
}
void DebugWindow::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		default:
		{
			BWindow::MessageReceived(message);
			break;
		}
	}
}
