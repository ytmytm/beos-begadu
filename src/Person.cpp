/*
	Person.cpp
	Ten plik jest częscią kodu źródłowego BeGadu.
	Homepage: http://gadu.beos.pl
*/

#include <stdlib.h>
#include <string.h>
#include <String.h>
#include <Path.h>
#include <File.h>
#include <FindDirectory.h>
#include <Alert.h>
#include <StorageKit.h>
#include "Person.h"
extern "C" {
#include "xmalloc.h"
#include "dynstuff.h"
#include "libgadu.h"
#include "strlcpy.h"
}

#ifndef PATH_MAX
#  define PATH_MAX _POSIX_PATH_MAX
#endif

Person::Person()
	{
	iName = new BString( "" );
	iSurname = new BString( "" );
	iNick = new BString( "" );
	iDisplay = new BString( "" );
	iMobile = new BString( "" );
	iEmail = new BString( "" );
	iDescription = new BString( "" );
	iLastDescr = new BString( "" );
	}

Profile::Profile()
	{
	iRect.left = 50;
	iRect.top = 50;
	iRect.right = 300;
	iRect.bottom = 350;
	iName = new BString();
	iName->SetTo( "" );
	iSurname = new BString();
	iSurname->SetTo( "" );
	iNick = new BString();
	iNick->SetTo( "" );
	iMobile = new BString();
	iMobile->SetTo( "" );
	iEmail = new BString();
	iEmail->SetTo( "" );
	iProfileName = new BString();
	iProfileName->SetTo( "Nowy" );
	iNumber = 0;
	iPassword = new BString();
	iPassword->SetTo( "" );
	iUserlist = new Userlist();
	iAutoStatus = GG_STATUS_AVAIL;
	iNeedImport = true;
	iDescription = new BString();
	iDescription->SetTo( "" );
	}

void Profile::SetUIN( uin_t aNumber )
	{
	iNumber = aNumber;
	}

void Profile::SetPass( BString* aPassword )
	{
	iPassword = aPassword;
	}

void Profile::SetName( BString* aProfileName )
	{
	iProfileName = aProfileName;
	}

void Profile::SetRect( BRect aRect )
	{
	iRect = aRect;
	}

void Profile::SetDesc( BString* aDescription )
	{
	iDescription = aDescription;
	}

void Profile::Load( BString* aProfile )
	{
	fprintf( stderr, "Profile::Load(%s)\n", aProfile->String() );
	BPath path;
	BMessage *cfgmesg = new BMessage();
	find_directory( B_USER_SETTINGS_DIRECTORY, &path );
	path.Append( "BeGadu/Profiles" );
	path.Append( aProfile->String() );
	path.Append( "Profile" );
	BFile file( path.Path(), B_READ_ONLY );
	if( file.InitCheck() == B_OK )
		{
		cfgmesg->Unflatten( &file );
		file.Unset();
		}
	if( cfgmesg->FindRect( "iRect", &iRect ) != B_OK )
		iRect =  BRect(50,50,300,350);
	if( cfgmesg->FindInt32( "iNumber", ( int32* ) &iNumber ) != B_OK )
		iNumber = 0;
	if( cfgmesg->FindString( "iPassword", iPassword ) != B_OK )
		iPassword->SetTo( "" );
	if( cfgmesg->FindInt32( "iAutoStatus", ( int32* ) &iAutoStatus ) != B_OK )
		iAutoStatus = GG_STATUS_AVAIL;
	if( cfgmesg->FindString( "iName", iName) != B_OK )
		iName->SetTo( "" );
	if( cfgmesg->FindString( "iSurname", iSurname ) != B_OK )
		iSurname->SetTo( "" );
	if( cfgmesg->FindString( "iNick", iNick ) != B_OK )
		iNick->SetTo( "" );
	if( cfgmesg->FindString( "iProfileName", iProfileName ) != B_OK )
		iProfileName->SetTo( "" );
	if( cfgmesg->FindString( "iMobile", iMobile ) != B_OK )
		iMobile->SetTo( "" );
	if( cfgmesg->FindString( "iEmail", iEmail ) != B_OK )
		iEmail->SetTo( "" );
	if( cfgmesg->FindBool( "iNeedImport", &iNeedImport) != B_OK )
		iNeedImport = true;
	delete cfgmesg;
	fprintf(stderr, "Loading %s profile...\n", path.Path());
	iUserlist->Read( iProfileName );
	}

void Profile::Save()
	{
	fprintf( stderr, "Profile::Save()\n" );
	BMessage *cfgmesg = new BMessage();
	cfgmesg->AddRect( "iRect", iRect );
	cfgmesg->AddInt32( "iNumber", iNumber);
	cfgmesg->AddString( "iPassword", *iPassword );
	cfgmesg->AddInt32( "iAutoStatus", iAutoStatus );
	cfgmesg->AddString( "iName", *iName );
	cfgmesg->AddString( "iSurname", *iSurname );
	cfgmesg->AddString( "iNick", *iNick );
	cfgmesg->AddString( "iProfileName", *iProfileName );
	cfgmesg->AddString( "iMobile", *iMobile );
	cfgmesg->AddString( "iEmail", *iEmail );
	cfgmesg->AddBool( "iNeedImport", iNeedImport );
	BPath path;
	BEntry entry;
	find_directory( B_USER_SETTINGS_DIRECTORY, &path );
	path.Append( "BeGadu/Profiles");
	BDirectory* profile = new BDirectory( path.Path() );
	if( profile->FindEntry( iProfileName->String(), &entry ) != B_OK )
		{
		path.Append( iProfileName->String());
		profile->CreateDirectory( path.Path(), profile );
		}
	find_directory( B_USER_SETTINGS_DIRECTORY, &path );
	path.Append( "BeGadu/Profiles" );
	path.Append( iProfileName->String() );
	path.Append( "Profile" );
	BFile file( path.Path(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE );
	if( file.InitCheck() == B_OK )
		{
		cfgmesg->Flatten( &file );
		file.Unset();
		}
	delete cfgmesg;
	iUserlist->Write( iProfileName );
	}

Userlist* Profile::GetUserlist() const
	{
	return iUserlist;
	}

BString* Profile::ProfileName() const
	{
	return iProfileName;
	}
		
int Profile::AutoStatus()
	{
	return iAutoStatus;
	}
	
Userlist::Userlist()
	{
	/* i think that nobody have >512 people in the list :P */
	iIsValid = false;
	iList = new List( 512 );
	}

/* Wczytuje userliste z pliku w formacie eksportu z servera */
/* Kod w większości ze źródeł ekg/userlist.[c,h] */
int Userlist::Read( BString* aName )
	{
	BPath 		path;
	FILE	*	plik;
	char	*	buf;
	find_directory( B_USER_SETTINGS_DIRECTORY, &path );
	path.Append( "BeGadu/Profiles" );
	path.Append( aName->String() );
	path.Append( "Userlist" );
	
	if( !( plik = fopen( path.Path(), "r" ) ) )
		return -1;
	Person *person = NULL;

	while( buf = ReadLine( plik ) )
		{
		person = new Person();
		char **entry, *uin;
		int i, count;
		
		if( buf[ 0 ] == '#' || ( buf[ 0 ] == '/' && buf[ 1 ] == '/' ) )
			{
			xfree( buf );
			continue;
			}
		entry = array_make( buf, ";", 9, 0, 0 );
		if( ( count = array_count( entry ) ) < 7 )
			{
			array_free( entry );
			continue;
			}
		uin = entry[ 6 ];
		
		if( ( strcmp( uin, "" ) && !( person->iUIN = atoi( uin ) ) ) )
			{
			array_free( entry );
			xfree( buf );
			continue;
			}
		person->iName->SetTo( entry[ 0 ] );
		person->iSurname->SetTo( entry[ 1 ] );
		person->iNick->SetTo( entry[ 2 ] );
		person->iDisplay->SetTo( entry[ 3 ] );
		person->iMobile->SetTo( entry[ 4 ] );
		person->iStatus = GG_STATUS_NOT_AVAIL;
		/* mamy mejla ? */
		if( count > 7 )
			person->iEmail->SetTo( entry[ 7 ] );
		for( i = 0; i < count; i++ )
			xfree( entry[ i ] );
		iList->AddItem( person );
		xfree( entry );
		xfree( buf );
		}
	SetValid( true );
	fclose( plik );
	return 0;
	}

/* Zapisuje userliste do pliku w formacie eksportu z servera */
/* Kod w większości ze źródeł ekg/userlist.[c,h] */
int Userlist::Write( BString* aName )
	{
	BPath 		path;
	FILE	*	plik;
	find_directory( B_USER_SETTINGS_DIRECTORY, &path );
	path.Append( "BeGadu/Profiles" );
	path.Append( aName->String() );
	path.Append( "Userlist" );
	char *	contacts, tmp[ PATH_MAX ];
	
	if( !( contacts = Dump() ) )
		return -1;
	snprintf( tmp, sizeof( tmp ), "%s.%d.%ld", path.Path(), ( int ) getpid(), ( long ) time( NULL ) );
	if( !( plik = fopen( tmp, "w" ) ) )
		{
		xfree( contacts );
		return -2;
		}
	fputs( contacts, plik );
	if( fclose( plik ) == EOF )
		{
		unlink( tmp );
		return -2;
		}
	xfree( contacts );
	if( rename( tmp, path.Path() ) == -1 )
		return -2;
	return 0;
	}

/* Czyści status danej osobie */
/* Kod w większości ze źródeł ekg/userlist.[c,h] */
void Userlist::ClearStatus( Person* aPerson )
	{
	Person *o = NULL;

	for( int i = 0; i < iList->CountItems(); i++ )
		{
		o = ( Person* ) iList->ItemAt( i );
		if( !aPerson->iUIN || aPerson->iUIN == o->iUIN )
			{
			o->iStatus = GG_STATUS_NOT_AVAIL;
			memset( &o->iIP, 0, sizeof( struct in_addr ) );
			memset( &o->iLastIP, 0, sizeof( struct in_addr ) );
			o->iPort = 0;
			o->iLastPort = 0;
			o->iDescription->SetTo( "" );
			o->iLastDescr->SetTo( "" );
			break;
			}
		}				
	}

/* Dodaje osobę o numerze i ustawia jej nazwę wyświetlania */
/* Kod w większości ze źródeł ekg/userlist.[c,h] */
void Userlist::Add( uin_t aUin, 
					BString* aName,
					BString* aSurname,
					BString* aNick,
					BString* aDisplay,
					BString* aMobile,
					BString* aEmail )
	{
	Person *o = new Person();
	o->iUIN = aUin;
	o->iName = aName;
	o->iSurname = aSurname;
	o->iNick = aNick;
	o->iDisplay = aDisplay;
	o->iMobile = aMobile;
	o->iStatus = GG_STATUS_NOT_AVAIL;
	o->iEmail = aEmail;
	iList->AddItem( o );
	}

/* Usuwa daną osobę z listy */
/* Kod w większości ze źródeł ekg/userlist.[c,h] */
void Userlist::Remove( Person* aPerson )
	{
	Person *o = NULL;

	for( int i = 0; i < iList->CountItems(); i++ )
		{
		o = ( Person* ) iList->ItemAt( i );
		if( o->iUIN == aPerson->iUIN )
			{
			iList->RemoveItem( o );
			break;
			}
		}
	}

/* Podmienia osobę na liście */
/* Kod w większości ze źródeł ekg/userlist.[c,h] */
void Userlist::Replace( Person* aPerson )
	{
	}

/* Wysyła listę na server */
/* Kod w większości ze źródeł ekg/userlist.[c,h] */
void Userlist::Send( struct gg_session* aSession )
	{
	Person 	*	o = NULL;
	uin_t 	*	numbers;
	int			count;
	count = iList->CountItems();
	numbers = ( uin_t* ) malloc( count * sizeof( uin_t ) );
	for( int i = 0; i < iList->CountItems(); i++ )
		{
		o = ( Person* ) iList->ItemAt( i );		
		numbers[ i ] = o->iUIN;
		}
	gg_notify( aSession, numbers, count );
	}

/* Szuka osoby po numerze i nazwie wyświetlanej */
/* Kod w większości ze źródeł ekg/userlist.[c,h] */
Person*	Userlist::Find( uin_t aUIN )
	{
	Person *o = NULL;
	for( int i = 0; i < iList->CountItems(); i++ )
		{
		o = ( Person* ) iList->ItemAt( i );
		if( aUIN && o->iUIN == aUIN )
			return o;
		}
	return NULL;
	}

/* Szuka osoby po numerze telefonu */
/* Kod w większości ze źródeł ekg/userlist.[c,h] */
Person* Userlist::FindMobile( const char* aMobile )
	{
	Person* o = NULL;
	for( int i = 0; i < iList->CountItems(); i++ )
		{
		o = ( Person* ) iList->ItemAt( i );
		if( aMobile && o->iMobile && !strcasecmp( o->iMobile->String(), aMobile ) )
			return o;
		}
	return NULL;
	}

/* Zrzuca listę w postaci tekstowej */
char* Userlist::Dump()
	{
	string_t s = string_init( NULL );
	Person* o = NULL;
	char *line;
	for( int i = 0; i < iList->CountItems(); i++ )
		{
		o = ( Person* ) iList->ItemAt( i );
		line = saprintf( "%s;%s;%s;%s;%s;%s;%s;%s%s\r\n",
						 o->iName->String(),
						 o->iSurname->String(),
						 o->iNick->String(),
						 o->iDisplay->String(),
						 o->iMobile->String(),
						 strdup( "" ),
						 itoa( o->iUIN ),
						 o->iEmail->String(),
						 strdup( "" ) );
		string_append( s, line );
		xfree( line );
		}
	return string_free( s, 0 );
	}

/* Czyści listę */
void Userlist::Clear()
	{
	Person* o = NULL;
	for( int i = 0; i < iList->CountItems(); i++ )
		{
		o = ( Person* ) iList->ItemAt( i );
		iList->RemoveItem( o );
		}
	}

/* Ustawia listę */
/* Kod w większości ze źródeł ekg/userlist.[c,h] */
int* Userlist::Set( const char* aContacts )
	{
	fprintf( stderr, "Userlist::Set() started\n" );
	char *buf, *cont, *contsave;
	char **entry, *uin;
	Clear();
	
	cont = contsave = strdup( aContacts );
	while( ( buf = gg_get_line( &cont ) ) )
		{
		Person *o = new Person();
		int i, count;

		if( buf[ 0 ] == '#' || ( buf[ 0 ] == '/' && buf[ 1 ] == '/' ) )
			continue;
		
		entry = array_make( buf, ";", 8, 0, 0 );
		if( ( count = array_count( entry ) < 7 ) )
			{
			array_free( entry );
			continue;
			}
		o->iUIN = atoi( entry[ 6 ] );
		o->iName->SetTo( entry[ 0 ] );
		o->iSurname->SetTo( entry[ 1 ]);
		o->iNick->SetTo( entry[ 2 ] );
		o->iDisplay->SetTo( entry[ 3 ] );
		o->iMobile->SetTo( entry[ 4 ] );
		o->iStatus = GG_STATUS_NOT_AVAIL;
		if( count > 7 )
			o->iEmail->SetTo( entry[ 7 ] );
		Add( o->iUIN, o->iName, o->iSurname, o->iNick, o->iDisplay, o->iMobile, o->iEmail );
		array_free( entry );
		delete o;
		}
	iIsValid = true;
//	Dump();
	}

/* Formatuje */
/* Kod w większości ze źródeł ekg/userlist.[c,h] */
const char* Userlist::FormatUser( uin_t aUIN )
	{
	}

/* Wczytuje/zwraca linijkę tekstu z pliku */
/* Kod w większości ze źródeł ekg */
char* Userlist::ReadLine( FILE *aFile )
	{
	char buf[ 1024 ], *res = NULL;

	while( fgets( buf, sizeof( buf ), aFile ) )
		{
		int first = ( res ) ? 0 : 1;
		size_t new_size = ( ( res ) ? strlen( res ) : 0 ) + strlen( buf ) + 1;

		res = ( char* ) xrealloc( res, new_size );
		if( first )
			*res = 0;
		strlcpy( res + strlen( res ), buf, new_size - strlen( res ) );

		if( strchr( buf, '\n' ) )
			break;
		}

	if( res && strlen( res ) > 0 && res[ strlen( res ) - 1 ] == '\n' )
		res[ strlen( res ) - 1 ] = 0;
	if( res && strlen( res ) > 0 && res[ strlen( res ) - 1 ] == '\r' )
		res[ strlen( res ) - 1 ] = 0;

	return res;
	}

bool Userlist::IsValid()
	{
	return iIsValid;
	}

void Userlist::SetValid( bool aValid )
	{
	iIsValid = aValid;
	}

void Userlist::Import(struct gg_session* aSession, List* aList )
	{
	char *blad;
	SetValid( false );
	if( gg_userlist_request( aSession, GG_USERLIST_GET, NULL ) == -1 )
		{
		sprintf( blad, "Błąd pobierania listy kontaktów: %s", strerror( errno ) );
		BAlert *alert = new BAlert( "Lista", blad, "OK" );
		alert->Go();
		}
	else
		{
		Person *person;
		for( int i = 0; i < aList->CountItems(); i++ )
			{
			person = ( Person* ) aList->ItemAt( i );
			aList->RemoveItem( person );
			delete person;
			}
		}
	fprintf( stderr, "import exit\n" );
	}

void Userlist::Export( struct gg_session* aSession, List* aList )
	{
	}

List* Userlist::GetList() const
	{
	return iList;
	}

List::List( int aSize ) : BList( aSize )
	{
	}

bool List::IsValid()
	{
	return iIsValid;
	}

void List::SetValid( bool aIsValid )
	{
	iIsValid = aIsValid;
	}
