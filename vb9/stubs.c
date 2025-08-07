/* Stub implementations for Plan 9 functions */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>

/* Plan 9 compatibility stubs */
void sysfatal(char *msg) { fprintf(stderr, "FATAL: %s\n", msg); exit(1); }
void* mallocz(size_t size, int zero) { void *p = malloc(size); if(zero && p) memset(p, 0, size); return p; }
void exits(char *msg) { exit(msg ? 1 : 0); }
int print(char *fmt, ...) { va_list args; va_start(args, fmt); int n = vprintf(fmt, args); va_end(args); return n; }
int snprint(char *buf, int len, char *fmt, ...) { va_list args; va_start(args, fmt); int n = vsnprintf(buf, len, fmt, args); va_end(args); return n; }
char* strdup(const char *s) { if(!s) return NULL; size_t len = strlen(s) + 1; char *p = malloc(len); if(p) memcpy(p, s, len); return p; }
