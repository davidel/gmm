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

#if !defined(_GMM_H)
#define _GMM_H


#ifdef  __cplusplus
#define GMMAPI extern "C"
#else
#define GMMAPI
#endif

#ifndef RTLD_NEXT
#define RTLD_NEXT ((void *) -1L)
#endif

#define GMM_REAL_LIBC RTLD_NEXT


#define GMM_EXECENV "GMM_FAULT_EXEC"
#define SSTK_SIZE (1024 * 4)
#define SSTK_EXTRA 64
#define GMM_MAX_STACK_DISP (1024 * 64)
#define GMM_PANIC() *(int *) 0 = 0


struct sstk_frame {
	char *ptr;
	int size;
};

#define GMM_FETCH_FRAME(res, tos, n) do { \
	long disp; \
	char *cur; \
	cur = (char *) __builtin_frame_address(n); \
	disp = (long) (cur - tos); \
	if (disp < -GMM_MAX_STACK_DISP || disp > GMM_MAX_STACK_DISP) \
		res = 0; \
	else { \
		res = 1; \
		tos = cur; \
	} \
} while (0)

#define SSTK_VARS \
	struct sstk_frame frm; \
	int cres; \
	void *ra[3]; \
	char sstk[SSTK_SIZE]

#define SSTK_LOAD_RAS \
	do { \
		int res; \
		char *tos; \
		ra[0] = ra[1] = ra[2] = NULL; \
		tos = (char *) __builtin_frame_address(0); \
		GMM_FETCH_FRAME(res, tos, 1); \
		if (!res || !(ra[0] = __builtin_return_address(1))) \
			break; \
		GMM_FETCH_FRAME(res, tos, 2); \
		if (!res || !(ra[1] = __builtin_return_address(2))) \
			break; \
		GMM_FETCH_FRAME(res, tos, 3); \
		if (!res || !(ra[2] = __builtin_return_address(3))) \
			break; \
	} while (0)

#define SSTK_CMP_RAS(r) \
	do { \
		r = 1; \
		if (ra[0] && ra[0] != __builtin_return_address(1)) \
			break; \
		if (ra[1] && ra[1] != __builtin_return_address(2)) \
			break; \
		if (ra[2] && ra[2] != __builtin_return_address(3)) \
			break; \
		r = 0; \
	} while (0)

#define SSTK_PRE \
	SSTK_LOAD_RAS; \
	gmm_copy_stack(sstk, __builtin_frame_address(0), SSTK_SIZE, &frm)

#define SSTK_POST \
	SSTK_CMP_RAS(cres); \
	if (cres) { \
		gmm_memcpy(frm.ptr, sstk, frm.size); \
		gmm_handle_overflow(); \
		GMM_PANIC(); \
	}

#define WRAP_FUNCTION(rt, name, pdecl, pcall) \
	rt name pdecl { \
		static rt (*func) pdecl = NULL; \
		rt ret; \
		SSTK_VARS; \
		if (!func) \
			func = (typeof(func)) dlsym(GMM_REAL_LIBC, #name ); \
		SSTK_PRE; \
		ret = func pcall ; \
		SSTK_POST; \
		return ret; \
	}

#define WRAP_VOID_FUNCTION(name, pdecl, pcall) \
	void name pdecl { \
		static void (*func) pdecl = NULL; \
		SSTK_VARS; \
		if (!func) \
			func = (typeof(func)) dlsym(GMM_REAL_LIBC, #name ); \
		SSTK_PRE; \
		func pcall ; \
		SSTK_POST; \
	}

#define WRAP_VFUNCTION(rt, name, pdecl, lst, pcall) \
	rt name pdecl { \
		static rt (*func) pdecl = NULL; \
		va_list args; \
		rt ret; \
		SSTK_VARS; \
		if (!func) \
			func = (typeof(func)) dlsym(GMM_REAL_LIBC, "v" #name ); \
		SSTK_PRE; \
		va_start(args, lst); \
		ret = func pcall ; \
		va_end(args); \
		SSTK_POST; \
		return ret; \
	}

#define WRAP_USER_FUNCTION(rt, name, pdecl, pcall) \
	rt name pdecl { \
		rt ret; \
		SSTK_VARS; \
		SSTK_PRE; \
		ret = name ## _gmm pcall ; \
		SSTK_POST; \
		return ret; \
	}

#define WRAP_USER_VOID_FUNCTION(name, pdecl, pcall) \
	void name pdecl { \
		SSTK_VARS; \
		SSTK_PRE; \
		name ## _gmm pcall ; \
		SSTK_POST; \
	}

#define WRAP_USER_VFUNCTION(rt, name, pdecl, lst, pcall) \
	rt name pdecl { \
		va_list args; \
		rt ret; \
		SSTK_VARS; \
		SSTK_PRE; \
		va_start(args, lst); \
		ret = name ## _gmm pcall ; \
		va_end(args); \
		SSTK_POST; \
		return ret; \
	}


GMMAPI void gmm_handle_overflow(void);
GMMAPI char *gmm_get_tos(void);
GMMAPI void *gmm_memcpy(void *dest, void const *src, int n);
GMMAPI void gmm_copy_stack(char *dest, char const *src, int size,
			   struct sstk_frame *frm);

#endif

