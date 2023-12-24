/*    Copyright 2023 Davide Libenzi
 * 
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *        http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 * 
 */


#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gmm.h"

#if !defined(MAX_PATH)
#define MAX_PATH 512
#endif

#define GMM_NEXT_FRAME(tos, n) do { \
	int res; \
	GMM_FETCH_FRAME(res, tos, n); \
	if (!res) \
		return tos; \
} while (0)


static char *gmm_exe_path(pid_t pid);



static char *gmm_exe_path(pid_t pid) {
	int err;
	char *buf, *exep;
	int (*psnprintf)(char *, size_t, const char *, ...);

	if (!(buf = (char *) malloc(MAX_PATH)))
		return NULL;
	if (!(exep = (char *) malloc(MAX_PATH))) {
		free(buf);
		return NULL;
	}
	psnprintf = (typeof(psnprintf)) dlsym(GMM_REAL_LIBC, "snprintf");
	(*psnprintf)(buf, MAX_PATH - 1, "/proc/%u/exe", (unsigned int) pid);
	err = readlink(buf, exep, MAX_PATH);
	free(buf);
	if (err < 0)
		free(exep), exep = NULL;

	return exep;
}

void gmm_handle_overflow(void) {
	int status;
	pid_t pid, ppid;
	char const *nex;
	int (*psnprintf)(char *, size_t, const char *, ...);
	char *argv[4];
	char pbuf[32];

	ppid = getpid();
	psnprintf = (typeof(psnprintf)) dlsym(GMM_REAL_LIBC, "snprintf");
	if ((nex = getenv(GMM_EXECENV)) != NULL) {
		if (!(pid = fork())) {
			(*psnprintf)(pbuf, sizeof(pbuf) - 1, "%u",
				     (unsigned int) ppid);
			argv[0] = strdup(nex);
			if (!(argv[1] = gmm_exe_path(ppid)))
				argv[1] = "";
			argv[2] = pbuf;
			argv[3] = NULL;

			execvp(argv[0], argv);
			exit(errno);
		}
		if (pid != -1)
			waitpid(pid, &status, 0);
	}
}

char *gmm_get_tos(void) {
	char *tos;

	tos = __builtin_frame_address(0);
	GMM_NEXT_FRAME(tos, 1);
	GMM_NEXT_FRAME(tos, 2);
	GMM_NEXT_FRAME(tos, 3);
	GMM_NEXT_FRAME(tos, 4);
	GMM_NEXT_FRAME(tos, 5);
	GMM_NEXT_FRAME(tos, 6);
	GMM_NEXT_FRAME(tos, 7);
	GMM_NEXT_FRAME(tos, 8);
	GMM_NEXT_FRAME(tos, 9);
	GMM_NEXT_FRAME(tos, 10);

	return tos;
}

void *gmm_memcpy(void *dest, void const *src, int n) {
	char *d;
	char const *s;

	if (dest < src) {
		d = dest;
		s = src;
		for (; n > 0; n--)
			*d++ = *s++;
	} else {
		d = (char *) dest + n - 1;
		s = (char const *) src + n - 1;
		for (; n > 0; n--)
			*d-- = *s--;
	}
	return dest;
}

void gmm_copy_stack(char *dest, char const *src, int size,
		    struct sstk_frame *frm) {
	char *tos = gmm_get_tos();

	frm->size = 0;
	frm->ptr = (char *) src;
	if (src && tos) {
		/*
		 * We try to handle even up-growing stacks, but the success
		 * is all but sure. Our bits might end up being screwed too
		 * in the overflow process.
		 */
		if (tos > (char *) &tos) {
			frm->size = tos - (char *) &tos - size - SSTK_EXTRA;
			frm->ptr = (char *) src;
		} else {
			frm->size = (char *) &tos - tos - size - SSTK_EXTRA;
			frm->ptr = (char *) src - frm->size;
		}
		if (frm->size > 0) {
			if (frm->size > size)
				frm->size = size;
			gmm_memcpy(dest, frm->ptr, frm->size);
		}
	}
}

