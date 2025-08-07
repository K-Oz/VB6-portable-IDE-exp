/* Minimal VB9 header for testing */
#ifndef VB9_H
#define VB9_H
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
typedef struct { int x, y; } Point;
typedef struct VB9Form VB9Form;
typedef struct Control Control;
enum { VB9_BUTTON = 2, VB9_TEXTBOX = 3, VB9_LABEL = 5 };
void sysfatal(char *msg);
void* mallocz(size_t size, int zero);
void exits(char *msg);
int print(char *fmt, ...);
int snprint(char *buf, int len, char *fmt, ...);
char* strdup(const char *s);
#define nil NULL
#endif
