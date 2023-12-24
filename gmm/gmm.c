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




WRAP_VOID_FUNCTION(bcopy, (const void *src, void *dest, size_t n), (src, dest, n))

WRAP_FUNCTION(void *, memmove, (void *dest, void const *src, size_t n), (dest, src, n))

WRAP_FUNCTION(void *, memcpy, (void *dest, void const *src, size_t n), (dest, src, n))

WRAP_FUNCTION(char *, strcpy, (char *dest, const char *src), (dest, src))

WRAP_FUNCTION(char *, strncpy, (char *dest, const char *src, size_t n), (dest, src, n))

WRAP_FUNCTION(char *, strcat, (char *dest, const char *src), (dest, src))

WRAP_FUNCTION(char *, strncat, (char *dest, const char *src, size_t n), (dest, src, n))

WRAP_FUNCTION(size_t, read, (int fd, void *buf, size_t count), (fd, buf, count))

WRAP_FUNCTION(size_t, fread, (void *ptr, size_t size, size_t nmemb, FILE *stream), (ptr, size, nmemb, stream))

WRAP_FUNCTION(char *, fgets, (char *s, int size, FILE *stream), (s, size, stream))

WRAP_FUNCTION(char *, gets, (char *s), (s))

WRAP_VFUNCTION(int, sprintf, (char *str, const char *fmt, ...), fmt, (str, fmt, args))

WRAP_VFUNCTION(int, snprintf, (char *str, size_t n, const char *fmt, ...), fmt, (str, n, fmt, args))

WRAP_FUNCTION(int, vsprintf, (char *str, const char *fmt, va_list args), (str, fmt, args))

WRAP_FUNCTION(int, vsnprintf, (char *str, size_t n, const char *fmt, va_list args), (str, n, fmt, args))

WRAP_VFUNCTION(int, fscanf, (FILE *stream, const char *fmt, ...), fmt, (stream, fmt, args))

WRAP_VFUNCTION(int, sscanf, (const char *str, const char *fmt, ...), fmt, (str, fmt, args))

WRAP_FUNCTION(char *, getwd, (char *s), (s))

WRAP_FUNCTION(char *, getcwd, (char *s, size_t n), (s, n))

