/* $Id$ */

/*
 *  (C) Copyright 2001-2003 Wojtek Kaniewski <wojtekka@irc.pl>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License Version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <sys/types.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "libgadu.h"

void *xcalloc(size_t nmemb, size_t size)
{
	void *tmp = calloc(nmemb, size);
	return tmp;
}

void *xmalloc(size_t size)
{
	void *tmp = malloc(size);
	/* na wszelki wypadek wyczy¶æ bufor */
	memset(tmp, 0, size);
	
	return tmp;
}

void xfree(void *ptr)
{
	if (ptr)
		free(ptr);
}

void *xrealloc(void *ptr, size_t size)
{
	void *tmp = realloc(ptr, size);
	return tmp;
}

char *xstrdup(const char *s)
{
	char *tmp;

	if (!s)
		return NULL;

	if (!(tmp = strdup(s)))
		fprintf(stderr,"Brak pamieci - xstrdup :(\n");
	return tmp;
}

char *saprintf(const char *format, ...)
{
	va_list ap;
	char *res;
	
	va_start(ap, format);
	res = gg_vsaprintf(format, ap);
	va_end(ap);

	if(!res)
		fprintf(stderr, "Brak pamieci - saprintf() :( \n");
	
	return res;
}
