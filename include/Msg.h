/*
 * ============================================================================
 *  Nazwa    : Msg z Msg.h
 *  Projekt  : BeGadu
 *  Authorzy : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Opis:
 *		Rozne stale i definicje
 *  Version  : 1.2
 * ============================================================================
 */

#ifndef __BEGADU_MSG_H__
#define __BEGADU_MSG_H__

/* messages from libgadu */
#define GOT_MESSAGE				'gome'
#define GOT_PEOPLE				'gope'
#define ADD_HANDLER				'adha'
#define DEL_HANDLER				'deha'

/* messages to interface */
#define UPDATE_STATUS			'upst'
#define UPDATE_LIST				'upli'
#define SHOW_MESSAGE			'shme'

/* messages from interface */
#define DO_LOGIN				'logi'
#define DO_LOGOUT				'logo'
#define ADD_PERSON				'adpe'
#define DEL_PERSON				'depe'
#define OPEN_MESSAGE			'opme'
#define SEND_MESSAGE			'seme'
#define CLOSE_MESSAGE			'clme'

/* local messages from interface */
#define BEGG_PREFERENCES		'bepe'
#define BEGG_ABOUT				'beab'
#define BEGG_PERSON_SELECTED	'pese'
#define	BEGG_PERSON_ACTION		'peac'
#define BEGG_SEND				'bese'
#define SET_AVAIL				'sava'
#define SET_BRB					'sbrb'
#define	SET_INVIS				'sinv'
#define SET_NOT_AVAIL			'snav'
#define SET_DESCRIPTION			'sdes'
#define BEGG_IMPORT_LIST		'imli'
#define BEGG_EXPORT_LIST		'exli'
#define BEGG_QUIT				'bggq'

/* misc */
#define BEGG_CONNECTING		1234
#define PREFERENCES_OK		'peok'
#define PREFERENCES_CANCEL	'peca'
#define PROFILE_CREATED		'prcr'
#define OPEN_PROFILE_WIZARD 'oppw'
#define CONFIG_OK			'cfok'
#define ONLINE_SOUND		"BeGadu state changed"
#define MESSAGE_SOUND		"BeGadu new message"
#define SHOW_MAIN_WINDOW	'bsmw'
#define BGDESKBAR_CHSTATE	'chst'
#define ADD_MESSENGER		'adme'
#define DEL_MESSENGER		'deme'

#endif /* __BEGADU_MSG_H__ */
