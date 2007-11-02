#ifndef __BEGADU_GADULISTVIEW_H__
#define __BEGADU_GADULISTVIEW_H__

#include <ListView.h>

class MainWindow;

class GaduListView : public BListView
	{
	public:
		GaduListView( const MainWindow* aMainWindow, BRect aFrame, const char* aName );
		~GaduListView();
		void MouseDown( BPoint aWhere );

	private:
		const MainWindow* iMainWindow;
	};

#endif /* __BEGADU_GADULISTVIEW_H__ */
