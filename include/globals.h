#ifndef _GLOBALS_H
#define _GLOBALS_H

#define APP_NAME	"BeGadu"
#define APP_MIME	"application/x-vnd.BeGadu"
#define VERSION		"0.0"

#define _T(x) (x)
#define _(x) (x)

#define DEBUG

#ifdef DEBUG
#define DEBUG_TRACE(str) fprintf(stderr, str)
#else
#define DEBUG_TRACE(str)
#endif

#endif
