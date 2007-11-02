
#ifndef __BEGADU_DEBUG_H__
#define __BEGADU_DEBUG_H__

#include <Window.h>

class BTextView;

class DebugWindow : public BWindow
	{
	public:
		DebugWindow();
		void AddLog(char *entry);
		virtual void MessageReceived(BMessage *message);
	private:
		BTextView	*dview;
	};

#endif /* __BEGADU_DEBUG_H__ */
