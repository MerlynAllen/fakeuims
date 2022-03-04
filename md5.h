
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* $Id: md5.c,v 1.3 2006-05-01 16:57:31 quentin Exp $ */

/*
 * Implementation of the md5 algorithm as described in RFC1321 Copyright (C)
 * 2005 Quentin Carbonneaux <crazyjoke@free.fr>
 *
 * This file is part of md5sum.
 *
 * md5sum is a free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Softawre
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * md5sum is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should hav received a copy of the GNU General Public License along with
 * md5sum; if not, write to the Free Software Foundation, Inc., 51 Franklin
 * St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#define MD5_LEN 16

void md5(const uint8_t *initial_msg, size_t initial_len, uint8_t *digest);