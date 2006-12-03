/*
 * ============================================================================
 *  Nazwa    : Siec z Siec.h
 *  Projekt  : BeGadu
 *  Authorzy : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Opis:
 *		Glowny modul do obslugi sieci
 *  Version  : 1.2
 * ============================================================================
 */

#ifndef __BEGADU_NETWORK_H__
#define __BEGADU_NETWORK_H__

#include <Looper.h>
#include <libgadu.h>

/* zewnetrzne klasy, includowane w zrodle */
class MainWindow;
class ChatWindow;
class List;
class Person;
class Profile;

class Network : public BLooper
	{
	public:
		Network( Profile* aProfile, List* aList );
		virtual void Quit();
		virtual void MessageReceived( BMessage* aMessage );
		void GotWindow( MainWindow* aWindow );
		inline int GetIdent()
			{
			return iIdent;
			}
		inline int GetStatus()
			{
			return iStatus;
			}
		/* funkcje wywoływane z interfejsu */
		void Login();
		void Login( int aStatus );
		void Logout();
		void AddPerson( uin_t aWho );
		void RemovePerson( uin_t aWho );
		void GotMsg( uin_t aFrom, const char* aMessage );
		void SendMsg( uin_t aWho, const char* aMessage );
		struct gg_session* Session() const;
		void SetStatus( int aStatus );

		struct gg_session 	*	iSession;
		struct gg_login_params	iLoginParam;
		struct gg_event		*	iEvent;
		MainWindow			*	iWindow;
		int						iStatus;

	private:
		/* klasy globalne */
		Profile		* iProfile;
		List		* iList;
		
		/* klasy lokalne */
		BList 			* iHandlerList;
		BList			* iWinList;
		
		/* informacje o połączeniu */
		int						iId;
		int						iIdent;
		
		/* dodatki */
		ChatWindow	* GetMesgWinForUser( uin_t aWho );
		Person		* GetPersonForUser( uin_t aWho );
		
		/* te funkcje robią to co libgadu chce by było zrobione */
		void AddHandler( int fd, int cond, void* data );
		void RemoveHandler( int fd );
	};

#endif /* __BEGADU_NETWORK_H__ */
