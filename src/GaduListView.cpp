
#include <PopUpMenu.h>
#include <MenuItem.h>

#include "GaduListView.h"
#include "Main.h"
#include "Msg.h"
#include "globals.h"

GaduListView::GaduListView( const MainWindow* aMainWindow, BRect aFrame, const char* aName )
	: BListView( aFrame, aName, B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_FRAME_EVENTS | B_FULL_UPDATE_ON_RESIZE | B_NAVIGABLE )
	{
	iMainWindow = aMainWindow;
}

GaduListView::~GaduListView() {
	iMainWindow = NULL;
}

void GaduListView::MouseDown( BPoint aWhere ) {
	/**
	 * calling default mouse down event
	 */
	BListView::MouseDown( aWhere );

	/**
	 * checking that user pressed secondary mouse button
	 * to display buddy dropdown menu
	 */
	unsigned long buttons;
	BPopUpMenu *menu = new BPopUpMenu( "menu", true, true );
	menu->AddItem( new BMenuItem( _T("Edit"), new BMessage( BUDDY_EDIT ) ) );
	GetMouse( &aWhere, &buttons, false );
	if( buttons & B_SECONDARY_MOUSE_BUTTON ) {
			BMenuItem *selItem = (BMenuItem*) menu->Go( ConvertToScreen( aWhere ), false, true );
			if( selItem )
				BMessenger( iMainWindow ).SendMessage( selItem->Message() );
	}
}
