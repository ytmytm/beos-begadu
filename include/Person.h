/*
 * ============================================================================
 *  Nazwa    : Osoba z Osoba.h
 *  Projekt  : BeGadu
 *  Authorzy : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Opis:
 *		Klasy zwiazane z obsluga userlist i osob
 *  Version  : 1.2
 * ============================================================================
 */

#ifndef __BEGADU_PERSON_H__
#define __BEGADU_PERSON_H__

#include <List.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>

#include <libgadu.h>

class Person
	{
	public:
		Person();
		~Person();
		
		void SetName( const char* aName );
		const char* GetName() const;
		
		void SetSurname( const char* aSurname );
		const char* GetSurname() const;
		
		void SetNick( const char* aNick );
		const char* GetNick() const;
		
		void SetDisplay( const char* aDisplay );
		const char* GetDisplay() const;
		
		void SetMobile( const char* aMobile );
		const char* GetMobile() const;
		
		void SetEmail( const char* aEmail );
		const char* GetEmail() const;
		
		void SetUIN( uin_t aUIN );
		uin_t GetUIN() const;
		
		void SetStatus( int aStatus );
		int GetStatus() const;
		
		void SetDescription( const char* aDescription );
		const char* GetDescription() const;
		
		void SetIP( struct in_addr aIP );
		struct in_addr GetIP() const;
		
		void SetPort( unsigned short aPort );
		unsigned short GetPort() const;
		
		void SetProtocol( int aProtocol );
		int GetProtocol() const;
		
		void SetLastSeen( time_t aLastSeen );
		time_t GetLastSeen() const;
		
		void SetLastDescription( const char* aDescription );
		const char* GetLastDescription() const;
		
		void SetLastIP( struct in_addr aIP );
		struct in_addr GetLastIP() const;
		
		void SetLastPort( unsigned short aPort );
		unsigned short GetLastPort() const;
		
		void SetImageSize( int aImageSize );
		int GetImageSize() const;
	
	private:		
		BString			*	iName;					/* Imię */
		BString			*	iSurname;				/* Nazwisko */
		BString			*	iNick;					/* Pseudonim */
		BString			*	iDisplay;				/* Wyświetl jako */
		BString			*	iMobile;				/* Telefon */
		BString			*	iEmail;					/* E-Mail */
		uin_t				iUIN;					/* Numer GG */
		int					iStatus;				/* Aktualny status */
		BString			*	iDescription;					/* Aktualny opis */
		struct in_addr		iIP;					/* Adres IP osoby */
		unsigned short		iPort;					/* Port osoby */
		int					iProtocol;				/* Wersja protokołu */
		time_t				iLastSeen;				/* Jeśli jest niedost./ukryty, to od kiedy */
		BString			*	iLastDescription;		/* J.w. ostatni opis */
		struct in_addr		iLastIP;				/* J.w. ostatni IP */
		unsigned short		iLastPort;				/* J.w. ostatni port */
		int 				iImgSize;				/* Maksymalny rozmiar obrazków */
	};

class List : public BList
	{
	public:
		List( int aSize );
		bool IsValid();
		void SetValid( bool aIsValid );
	
	private:
		bool iIsValid;
	};

/* Klasa do obsługi userlisty */
class Userlist
	{
	public:
		Userlist();
		~Userlist();
		int				Read( BString* aName );			 /* Ładuje userliste z pliku */
		int				Write( BString* aName );			 /* Zapisuje ją do pliku */
		void			ClearStatus( Person* aPerson );		 /* Czyści status danej osobie */
		void			Add( uin_t aNumber,
							 const char* aName,
							 const char* aSurname,
							 const char* aNick,
							 const char* aDisplay,
							 const char* aMobile,
							 const char* aEmail ); 
		void			Remove( Person* aPerson );			 /* Usuwa osobę */
		void			Replace( Person* aPerson );			 /* Podmienia osobę */
		void			Import( struct gg_session* aSession, List* aList );
														 /* Importuje z servera */
		void			Export( struct gg_session* aSession, List* aList );
														 /* Eksportuje na server */
		void			Send( struct gg_session* aSession );	 /* Wysyła liste na server */
		bool			IsValid();
		void			SetValid( bool aValid );
		Person		*	Find( uin_t aNumber );					 /* Szuka osoby */
		Person		*	FindMobile( const char* aMobile );	 /* Szuka osoby po telefonie */
		char		*	Dump();							 /* Zrzuca */
		void			Clear();						 /* Czyści */
		void			Set( const char* aContacts ); 		 /* Ustawia */
		const char 	*	FormatUser( uin_t aUIN );			 /* Formatuje */
		char 		*	ReadLine( FILE* aFile );				 /* Wczytuje/zwraca linijke tekstu z pliku */
		List		*	GetList() const;
				
		List		*	iList;
		bool			iIsValid;
	};

class Profile
	{
	public:
		Profile();
		~Profile();
		
		int	Load( BString* aProfile );
		void Save();
		
		void SetUIN( uin_t aUIN );
		uin_t GetUIN() const;
		
		void SetName( const char* aName );
		const char* GetName() const;
		
		void SetMobile( const char* aMobile );
		const char* GetMobile() const;
		
		void SetEmail( const char* aEmail );
		const char* GetEmail() const;
		
		void SetDescription( const char* aDescription );
		const char* GetDescription() const;
		
		void SetPassword( const char* aPassword );
		const char* GetPassword() const;
		
		void SetSurname( const char* aSurname );
		const char* GetSurname() const;

		void SetNick( const char* aNick );
		const char* GetNick() const;
		
		void SetProfileName( const char* aProfileName );
		const char* GetProfileName() const;
		
		void SetProfilePassword( const char* aPassword );
		const char* GetProfilePassword() const;
		
		Userlist* GetUserlist() const;

		void SetRect( BRect aRect );
		BRect GetRect() const;
		
		void SetAutoStatus( int aAutoStatus );
		int GetAutoStatus() const;
		
		void SetNeedImport( bool aNeedImport );
		bool GetNeedImport() const;
		
		void SetRememberPassword( bool aRememberPassword );
		bool GetRememberPassword() const;

	private:
		BString	* iName;
		BString	* iMobile;
		BString	* iEmail;
		BString	* iDescription;
		BString * iPassword;
		BString	* iSurname;
		BString	* iNick;
		BString	* iProfileName;
		BString	* iProfilePassword;
		Userlist * iUserlist;
		BRect iRect;
		uin_t iUIN;
		int	iAutoStatus;
		bool iNeedImport;
		bool iRememberPassword;
	};

#endif /* __BEGADU_OSOBA_H__ */
