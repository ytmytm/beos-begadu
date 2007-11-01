
#ifndef __BEGADU_DEBUG_H__
#define __BEGADU_DEBUG_H__

#include <Window.h>

class TextView;

class DebugWindow : public BWindow
	{
	private:
		BTextView	*dview;
	public:
		DebugWindow();
		void AddLog(char *entry);
		virtual void MessageReceived(BMessage *message);
	};

#endif /* __BEGADU_DEBUG_H__ */
