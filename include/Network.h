
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
		/* funkcje wywoływane z interfejsu */
		void GotWindow( MainWindow* aWindow );
		void Login();
		void Login( int aStatus );
		void Login( int aStatus, BString *aDescription );
		void Logout();
		void AddPerson( uin_t aWho );
		void RemovePerson( uin_t aWho );
		void GotMsg( uin_t aFrom, const char* aMessage );
		void SendMsg( uin_t aWho, const char* aMessage );

		inline struct gg_session* Session() const { return iSession; }
		void SetSession( struct gg_session *s );

		inline int Status() { return iStatus; }
		void SetStatus( int aStatus );

		void SetDescription( BString *aDescription );

		MainWindow			*	iWindow;
		BString				*	iDescription;

		struct gg_login_params	iLoginParam;
		struct gg_event		*	iEvent;

	private:
		struct gg_session 	*	iSession;

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
		void AddHandler( int fd );
		void RemoveHandler( int fd );
	};

#endif /* __BEGADU_NETWORK_H__ */
