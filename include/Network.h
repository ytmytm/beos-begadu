
#ifndef __BEGADU_NETWORK_H__
#define __BEGADU_NETWORK_H__

#include <Looper.h>
#include <libgadu.h>

class MainWindow;
class ChatWindow;
class List;
class Person;
class Profile;

class Network : public BLooper
	{
	public:
		Network( Profile* aProfile );
		virtual void Quit();
		virtual void MessageReceived( BMessage* aMessage );
		void GotWindow( MainWindow* aWindow );
		/* funkcje wywoływane z interfejsu */
		void Login();
		void Login( int aStatus );
		void Login( int aStatus, BString *aDescription );
		void Logout();
		void AddPerson( uin_t aWho );
		void RemovePerson( uin_t aWho );
		void GotMsg( uin_t aFrom, const char* aMessage );
		void SendMsg( uin_t aWho, const char* aMessage );
		struct gg_session* Session() const;
		void SetStatus( int aStatus );
		void SetDescription( BString *aDescription );
		inline int GetStatus() { return iStatus; }

		MainWindow			*	iWindow;
		BString				*	iDescription;

		struct gg_session 	*	iSession;
		struct gg_login_params	iLoginParam;
		struct gg_event		*	iEvent;

	private:
		int						iStatus;

		/* klasy globalne */
		Profile		* iProfile;

		/* klasy lokalne */
		BList 			* iHandlerList;
		BList			* iWinList;

		/* dodatki */
		ChatWindow	* GetMesgWinForUser( uin_t aWho );
		Person		* GetPersonForUser( uin_t aWho );

		/* te funkcje robią to co libgadu chce by było zrobione */
		void AddHandler( int fd, int cond, void* data );
		void RemoveHandler( int fd );
	};

#endif /* __BEGADU_NETWORK_H__ */
