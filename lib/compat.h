/* $Id$ */

/*
 *  (C) Copyright 2001-2002 Wojtek Kaniewski <wojtekka@irc.pl>
 *                          Robert J. Wo¼ny <speedy@ziew.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License Version
 *  2.1 as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
 *  USA.
 */

#ifndef __COMPAT_H
#define __COMPAT_H
#include <netdb.h>
extern _IMPEXP_NET int getsockopt (int __fd, int __level, int __optname, void *__optval, int *__optlen);

typedef uint32 in_addr_t;
#  define INADDR_NONE   ((in_addr_t) 0xffffffff)
#define SHUT_RDWR 0

#define SO_ERROR	4
#define SO_SNDBUF	7
#define SO_RCVBUF	8


#endif
