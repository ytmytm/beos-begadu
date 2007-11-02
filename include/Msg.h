
#ifndef __BEGADU_MSG_H__
#define __BEGADU_MSG_H__

/* messages from libgadu */
#define GOT_MESSAGE				'0000'
#define ADD_HANDLER				'0002'
#define DEL_HANDLER				'0003'

/* messages to interface */
#define UPDATE_STATUS			'0004'
#define UPDATE_LIST				'0005'
#define SHOW_MESSAGE			'0006'

/* messages from interface */
#define DO_LOGIN				'0007'
#define DO_LOGOUT				'0008'
#define ADD_BUDDY				'0009'
#define DEL_BUDDY				'0010'
#define OPEN_MESSAGE			'0011'
#define SEND_MESSAGE			'0012'
#define CLOSE_MESSAGE			'0013'

/* local messages from interface */
#define BEGG_PREFERENCES		'0014'
#define BEGG_ABOUT				'0015'
#define BEGG_PERSON_SELECTED	'0016'
#define	BEGG_PERSON_ACTION		'0017'
#define BEGG_SEND				'0018'
#define SET_AVAIL				'0019'
#define SET_BRB					'0020'
#define	SET_INVIS				'0021'
#define SET_NOT_AVAIL			'0022'
#define SET_DESCRIPTION			'0023'
#define BEGG_IMPORT_LIST		'0024'
#define BEGG_EXPORT_LIST		'0025'
#define BEGG_QUIT				'0026'

/* misc */
#define PREFERENCES_OK			'0028'
#define PREFERENCES_CANCEL		'0029'
#define PREFERENCES_SWITCH		'0030'
#define PROFILE_CREATED			'0032'
#define OPEN_PROFILE_WIZARD 	'0033'
#define CONFIG_OK				'0034'
#define SHOW_MAIN_WINDOW		'0035'
#define BGDESKBAR_CHSTATE		'0036'
#define ADD_MESSENGER			'0037'
#define DEL_MESSENGER			'0038'
#define CHANGE_DESCRIPTION		'0039'
#define DESCRIPTION_OK			'0040'
#define DESCRIPTION_CANCEL		'0041'
#define SET_CONN_DESCRIPTION	'0200'

/* preferences */
#define PROFILE_PASS_CHECK_BOX	'0042'
#define PROFILE_DELETE_ACCOUNT	'0043'
#define PROFILE_CHANGE_PASSWORD	'0044'

/* profiles */
#define PROFILE_SELECTED		'0090'
#define PROFILE_NOT_SELECTED	'0091'
#define PROFILE_SELECT			'0092'
#define PROFILE_CANCEL			'0093'
#define PROFILE_CHECK			'0094'
#define PROFILE_SWITCH			'0095'
#define CLOSE_MAIN_WINDOW		'0096'

/* buddies msgs */
#define BUDDY_EDIT				'0100'
#define BUDDY_EDIT_OK			'0101'
#define BUDDY_EDIT_CANCEL		'0102'

//#define ONLINE_SOUND			"BeGadu state changed"
//#define MESSAGE_SOUND			"BeGadu new message"

#endif /* __BEGADU_MSG_H__ */
