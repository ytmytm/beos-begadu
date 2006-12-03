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
		BString			*	iLastDescr;				/* J.w. ostatni opis */
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
						Userlist();						 /* Konstruktor */
		int				Read( BString* aName );			 /* Ładuje userliste z pliku */
		int				Write( BString* aName );			 /* Zapisuje ją do pliku */
		void			ClearStatus( Person* aPerson );		 /* Czyści status danej osobie */
		void			Add( uin_t aNumber,
							 BString* aName,
							 BString* aSurname,
							 BString* aNick,
							 BString* aDisplay,
							 BString* aMobile,
							 BString* aEmail ); 
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
		int			*	Set( const char* aContacts ); 		 /* Ustawia */
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
		void					Load( BString* aProfile );
		void					Save();
		void					SetUIN( uin_t aNumber );
		void					SetPass( BString* aPassword );
		void					SetName( BString* aName );
		void 					SetRect( BRect aRect );
		void					SetDesc( BString* aDescription );
		Userlist			*	GetUserlist() const;
		BString				*	ProfileName() const;
		int						AutoStatus();
		
		Userlist			*	iUserlist;
		BRect					iRect;
		uin_t					iNumber;
		BString				*	iPassword;
		int						iAutoStatus;
		BString				*	iName;
		BString				*	iSurname;
		BString				*	iNick;
		BString				*	iProfileName;
		BString				*	iMobile;
		BString				*	iEmail;
		BString				*	iDescription;
		bool					iNeedImport;
	};


#endif /* __BEGADU_OSOBA_H__ */
