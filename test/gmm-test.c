/*
 *  GMM by Davide Libenzi ( Guarded Memory Move )
 *  Copyright (C) 2004  Davide Libenzi
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Davide Libenzi <davidel@xmailserver.org>
 *
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>



static void do_overflow(char *str) {
	char buf[32];

	sprintf(buf, "%s", str);
	strcpy(buf, str);
}

static int run_test(char *str) {

	do_overflow(str);

	return 0;
}

int main(int argc, char **argv) {

	run_test(argv[1]);

	return 0;
}

