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
#include <libgadu.h>

#include "Person.h"
#include "globals.h"

extern "C" {
#include "xmalloc.h"
#include "dynstuff.h"
#include "strlcpy.h"
}

//#ifdef DEBUG
#define DEBUG_TRACE(str) fprintf(stderr, str)
//#else
//#define DEBUG_TRACE(str)
//#endif

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
	iLastDescription = new BString( "" );
	}

Person::~Person()
	{
	delete iName;
	delete iSurname;
	delete iNick;
	delete iDisplay;
	delete iMobile;
	delete iEmail;
	delete iDescription;
	delete iLastDescription;
	}

void Person::SetName( const char* aName )
	{
	if( aName != NULL )
		iName->SetTo( aName );
	}

const char* Person::GetName() const
	{
	return iName->String();
	}

void Person::SetSurname( const char* aSurname )
	{
	if( aSurname != NULL )
		iSurname->SetTo( aSurname );
	}

const char* Person::GetSurname() const
	{
	return iSurname->String();
	}

void Person::SetNick( const char* aNick )
	{
	if( aNick != NULL )
		iNick->SetTo( aNick );
	}

const char* Person::GetNick() const
	{
	return iNick->String();
	}

void Person::SetDisplay( const char* aDisplay )
	{
	if( aDisplay != NULL )
		iDisplay->SetTo( aDisplay );
	}

const char* Person::GetDisplay() const
	{
	return iDisplay->String();
	}

void Person::SetMobile( const char* aMobile )
	{
	if( aMobile != NULL )
		iMobile->SetTo( aMobile );
	}

const char* Person::GetMobile() const
	{
	return iMobile->String();
	}

void Person::SetEmail( const char* aEmail )
	{
	if( aEmail != NULL )
		iEmail->SetTo( aEmail );
	}

const char* Person::GetEmail() const
	{
	return iEmail->String();
	}

void Person::SetUIN( uin_t aUIN )
	{
	iUIN = aUIN;
	}

uin_t Person::GetUIN() const
	{
	return iUIN;
	}

void Person::SetStatus( int aStatus )
	{
	iStatus = aStatus;
	}

int Person::GetStatus() const
	{
	return iStatus;
	}

void Person::SetDescription( const char* aDescription )
	{
	if( aDescription != NULL )
		iDescription->SetTo( aDescription );
	}

const char* Person::GetDescription() const
	{
	return iDescription->String();
	}

void Person::SetIP( struct in_addr aIP )
	{
	iIP = aIP;
	}

struct in_addr Person::GetIP() const
	{
	return iIP;
	}

void Person::SetPort( unsigned short aPort )
	{
	iPort = aPort;
	}

unsigned short Person::GetPort() const
	{
	return iPort;
	}

void Person::SetProtocol( int aProtocol )
	{
	iProtocol = aProtocol;
	}

int Person::GetProtocol() const
	{
	return iProtocol;
	}

void Person::SetLastSeen( time_t aLastSeen )
	{
	iLastSeen = aLastSeen;
	}

time_t Person::GetLastSeen() const
	{
	return iLastSeen;
	}

void Person::SetLastDescription( const char* aDescription )
	{
	if( aDescription != NULL )
		iLastDescription->SetTo( aDescription );
	}

const char* Person::GetLastDescription() const
	{
	return iLastDescription->String();
	}

void Person::SetLastIP( struct in_addr aIP )
	{
	iLastIP = aIP;
	}

struct in_addr Person::GetLastIP() const
	{
	return iLastIP;
	}

void Person::SetLastPort( unsigned short aPort )
	{
	iLastPort = aPort;
	}

unsigned short Person::GetLastPort() const
	{
	return iLastPort;
	}

Profile::Profile()
	{
	iRect.left = 50;
	iRect.top = 50;
	iRect.right = 300;
	iRect.bottom = 350;
	iName = new BString( "" );
	iSurname = new BString( "" );
	iNick = new BString( "" );
	iMobile = new BString( "" );
	iEmail = new BString( "" );
	iProfileName = new BString( "Nowy" );
	iUIN = 0;
	iPassword = new BString( "" );
	iProfilePassword = new BString( "" );
	iUserlist = new Userlist();
	iAutoStatus = GG_STATUS_AVAIL;
	iNeedImport = true;
	iDescription = new BString( "" );
	iRememberPassword = true;
	}

Profile::~Profile()
	{
	delete iUserlist;
	delete iPassword;
	delete iProfilePassword;
	delete iName;
	delete iSurname;
	delete iNick;
	delete iProfileName;
	delete iMobile;
	delete iEmail;
	delete iDescription;
	}

void Profile::SetUIN( uin_t aUIN )
	{
	iUIN = aUIN;
	}

uin_t Profile::GetUIN() const
	{
	return iUIN;
	}

void Profile::SetName( const char* aName )
	{
	if( aName != NULL )
		iName->SetTo( aName );
	}

const char* Profile::GetName() const
	{
	return iName->String();
	}

void Profile::SetMobile( const char* aMobile )
	{
	if( aMobile != NULL )
		iMobile->SetTo( aMobile );
	}

const char* Profile::GetMobile() const
	{
	return iMobile->String();
	}

void Profile::SetEmail( const char* aEmail )
	{
	if( aEmail != NULL )
		iEmail->SetTo( aEmail );
	}

const char* Profile::GetEmail() const
	{
	return iEmail->String();
	}

void Profile::SetDescription( const char* aDescription )
	{
	if( aDescription != NULL )
		iDescription->SetTo( aDescription );
	}

const char* Profile::GetDescription() const
	{
	return iDescription->String();
	}

void Profile::SetPassword( const char* aPassword )
	{
	if( aPassword != NULL )
		iPassword->SetTo( aPassword );
	}

const char* Profile::GetPassword() const
	{
	return iPassword->String();
	}

void Profile::SetSurname( const char* aSurname )
	{
	if( aSurname != NULL )
		iSurname->SetTo( aSurname );
	}

const char* Profile::GetSurname() const
	{
	return iSurname->String();
	}

void Profile::SetNick( const char* aNick )
	{
	if( aNick != NULL )
		iNick->SetTo( aNick );
	}

const char* Profile::GetNick() const
	{
	return iNick->String();
	}

void Profile::SetProfileName( const char* aProfileName )
	{
	if( aProfileName != NULL )
		iProfileName->SetTo( aProfileName );
	}

const char* Profile::GetProfileName() const
	{
	return iProfileName->String();
	}

void Profile::SetProfilePassword( const char* aPassword )
	{
	if( aPassword != NULL )
		iProfilePassword->SetTo( aPassword );
	}

const char* Profile::GetProfilePassword() const
	{
	return iProfilePassword->String();
	}

Userlist* Profile::GetUserlist() const
	{
	return iUserlist;
	}

void Profile::SetRect( BRect aRect )
	{
	iRect = aRect;
	}

BRect Profile::GetRect() const
	{
	return iRect;
	}

void Profile::SetAutoStatus( int aAutoStatus )
	{
	iAutoStatus = aAutoStatus;
	}

int Profile::GetAutoStatus() const
	{
	return iAutoStatus;
	}

void Profile::SetNeedImport( bool aNeedImport )
	{
	iNeedImport = aNeedImport;
	}

bool Profile::GetNeedImport() const
	{
	return iNeedImport;
	}

void Profile::SetRememberPassword( bool aRememberPassword )
	{
	iRememberPassword = aRememberPassword;
	}

bool Profile::GetRememberPassword() const
	{
	return iRememberPassword;
	}

int Profile::Load( BString* aProfile )
	{
	DEBUG_TRACE( "Profile::Load()\n" );
	BPath path;
	BMessage *cfgmesg = new BMessage();
	find_directory( B_USER_SETTINGS_DIRECTORY, &path );
	path.Append( "BeGadu/Profiles" );
	BDirectory * bg_conf = new BDirectory( path.Path() );
	BEntry entry;
	if( bg_conf->FindEntry( aProfile->String(), &entry ) != B_OK )
		{
		return -1;
		}
	if( !entry.IsDirectory() )
		{
		return -2;
		}

	path.Append( aProfile->String() );
	bg_conf->SetTo( path.Path() );
	if( bg_conf->FindEntry( "Profile", &entry ) != B_OK )
		{
		return -3;
		}
		
	path.Append( "Profile" );
	BFile file( path.Path(), B_READ_ONLY );
	if( file.InitCheck() == B_OK )
		{
		cfgmesg->Unflatten( &file );
		file.Unset();
		}
	if( cfgmesg->FindRect( "iRect", &iRect ) != B_OK )
		iRect =  BRect(50,50,300,350);
	if( cfgmesg->FindInt32( "iUIN", ( int32* ) &iUIN ) != B_OK )
		iUIN = 0;
	if( cfgmesg->FindString( "iPassword", iPassword ) != B_OK )
		iPassword->SetTo( "" );
	if( cfgmesg->FindBool( "iRememberPassword", iRememberPassword ) != B_OK )
		iRememberPassword = true;
	if( cfgmesg->FindInt32( "iAutoStatus", ( int32* ) &iAutoStatus ) != B_OK )
		iAutoStatus = GG_STATUS_AVAIL;
	if( cfgmesg->FindString( "iName", iName) != B_OK )
		iName->SetTo( "" );
	if( cfgmesg->FindString( "iProfilePassword", iProfilePassword ) != B_OK )
		iProfilePassword->SetTo( "" );
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
	return 0;
	}

void Profile::Save()
	{
	DEBUG_TRACE( "Profile::Save()\n" );
	BMessage *cfgmesg = new BMessage();
	cfgmesg->AddRect( "iRect", iRect );
	cfgmesg->AddInt32( "iUIN", iUIN );
	cfgmesg->AddString( "iPassword", *iPassword );
	cfgmesg->AddBool( "iRememberPassword", iRememberPassword );
	cfgmesg->AddInt32( "iAutoStatus", iAutoStatus );
	cfgmesg->AddString( "iName", *iName );
	cfgmesg->AddString( "iProfilePassword", *iProfilePassword );
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
	fprintf(stderr, "Saving %s profile...\n", path.Path());
	iUserlist->Write( iProfileName );
	}

Userlist::Userlist()
	{
	/* i think that nobody have >512 people in the list :P */
	iIsValid = false;
	iList = new List( 512 );
	}

Userlist::~Userlist()
	{
	delete iList;
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
	while( (buf = ReadLine( plik )) )
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
		
		if( strcmp( uin, "" ) == 0 )
			{
			array_free( entry );
			xfree( buf );
			continue;
			}
		person->SetUIN( atoi( uin ) );
		person->SetName( entry[ 0 ] );
		person->SetSurname( entry[ 1 ] );
		person->SetNick( entry[ 2 ] );
		person->SetDisplay( entry[ 3 ] );
		person->SetMobile( entry[ 4 ] );
		person->SetStatus( GG_STATUS_NOT_AVAIL );
		/* mamy mejla ? */
		if( count > 7 )
			person->SetEmail( entry[ 7 ] );
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
	struct in_addr addr;

	for( int i = 0; i < iList->CountItems(); i++ )
		{
		o = ( Person* ) iList->ItemAt( i );
		if( !aPerson->GetUIN() || aPerson->GetUIN() == o->GetUIN() )
			{
			o->SetStatus( GG_STATUS_NOT_AVAIL );
			addr = o->GetIP();
			memset( &addr, 0, sizeof( struct in_addr ) );
			addr = o->GetLastIP();
			memset( &addr, 0, sizeof( struct in_addr ) );
			o->SetPort( 0 );
			o->SetLastPort( 0 );
			o->SetDescription( "" );
			o->SetLastDescription( "" );
			break;
			}
		}				
	}

/* Dodaje osobę o numerze i ustawia jej nazwę wyświetlania */
/* Kod w większości ze źródeł ekg/userlist.[c,h] */
void Userlist::Add( uin_t aUin, 
					const char* aName,
					const char* aSurname,
					const char* aNick,
					const char* aDisplay,
					const char* aMobile,
					const char* aEmail )
	{
	Person *o = new Person();
	o->SetUIN( aUin );
	o->SetName( aName );
	o->SetSurname( aSurname );
	o->SetNick( aNick );
	o->SetDisplay( aDisplay );
	o->SetMobile( aMobile );
	o->SetStatus( GG_STATUS_NOT_AVAIL );
	o->SetEmail( aEmail );
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
		if( o->GetUIN() == aPerson->GetUIN() )
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
		numbers[ i ] = o->GetUIN();
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
		if( aUIN && ( o->GetUIN() == aUIN ) )
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
		if( aMobile && o->GetMobile() && !strcasecmp( o->GetMobile(), aMobile ) )
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
						 o->GetName(),
						 o->GetSurname(),
						 o->GetNick(),
						 o->GetDisplay(),
						 o->GetMobile(),
						 strdup( "" ),
						 itoa( o->GetUIN() ),
						 o->GetEmail(),
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
void Userlist::Set( const char* aContacts )
	{
	DEBUG_TRACE( "Userlist::Set()\n" );
	char *buf, *cont, *contsave;
	char **entry;
	Clear();
	
	cont = contsave = strdup( aContacts );
	while( ( buf = gg_get_line( &cont ) ) )
		{
		Person *o = new Person();
		int count;

		if( buf[ 0 ] == '#' || ( buf[ 0 ] == '/' && buf[ 1 ] == '/' ) )
			continue;
		
		entry = array_make( buf, ";", 8, 0, 0 );
		if( ( count = array_count( entry ) < 7 ) )
			{
			array_free( entry );
			continue;
			}
		o->SetUIN( atoi( entry[ 6 ] ) );
		o->SetName( entry[ 0 ] );
		o->SetSurname( entry[ 1 ]);
		o->SetNick( entry[ 2 ] );
		o->SetDisplay( entry[ 3 ] );
		o->SetMobile( entry[ 4 ] );
		o->SetStatus( GG_STATUS_NOT_AVAIL );
		if( count > 7 )
			o->SetEmail( entry[ 7 ] );
		Add( o->GetUIN(),
			 o->GetName(),
			 o->GetSurname(),
			 o->GetNick(),
			 o->GetDisplay(),
			 o->GetMobile(),
			 o->GetEmail() );
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
		return NULL;
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
	SetValid( false );
	if( gg_userlist_request( aSession, GG_USERLIST_GET, NULL ) == -1 )
		{
		char blad[256];
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
	}

void Userlist::Export( struct gg_session* aSession, List* aList )
	{
		BAlert *alert = new BAlert( _T("Export"), _T("Not implemented yet"), _T("OK") );
		alert->Go();
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
