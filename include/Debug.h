/*
 * ============================================================================
 *  Nazwa    : Debug z Debug.h
 *  Projekt  : BeGadu
 *  Authorzy : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Opis:
 *		Modul do debugowania
 *  Version  : 1.2
 * ============================================================================
 */


#ifndef __BEGADU_DEBUG_H__
#define __BEGADU_DEBUG_H__

#include <Window.h>
#include <TextView.h>

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
