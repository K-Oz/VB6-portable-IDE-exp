# Linux Makefile for VB9 Advanced Features Testing

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude -g
LIBS = -lm

# Source files
COMMON_SRCS = src/neural.c src/quantum.c src/font-coding.c src/glyph-kernel.c
TEST_SRCS = test/test-vb9-advanced.c

# Stub implementations to avoid link errors
STUB_SRCS = stubs.c

# Targets
all: test-vb9-advanced glyph-kernel-standalone

glyph-kernel-standalone: glyph-kernel-standalone.c
	$(CC) $(CFLAGS) -o $@ glyph-kernel-standalone.c $(LIBS)

# Create stub implementations for Plan 9 specific functions
stubs.c:
	@echo "Creating stub implementations..."
	@echo '/* Stub implementations for Plan 9 functions */' > stubs.c
	@echo '#include <stdio.h>' >> stubs.c
	@echo '#include <stdlib.h>' >> stubs.c
	@echo '#include <string.h>' >> stubs.c
	@echo '#include <stdint.h>' >> stubs.c
	@echo '#include <stdarg.h>' >> stubs.c
	@echo '#include <stddef.h>' >> stubs.c
	@echo '' >> stubs.c
	@echo '/* Plan 9 compatibility stubs */' >> stubs.c
	@echo 'void sysfatal(char *msg) { fprintf(stderr, "FATAL: %s\\n", msg); exit(1); }' >> stubs.c
	@echo 'void* mallocz(size_t size, int zero) { void *p = malloc(size); if(zero && p) memset(p, 0, size); return p; }' >> stubs.c
	@echo 'void exits(char *msg) { exit(msg ? 1 : 0); }' >> stubs.c
	@echo 'int print(char *fmt, ...) { va_list args; va_start(args, fmt); int n = vprintf(fmt, args); va_end(args); return n; }' >> stubs.c
	@echo 'int snprint(char *buf, int len, char *fmt, ...) { va_list args; va_start(args, fmt); int n = vsnprintf(buf, len, fmt, args); va_end(args); return n; }' >> stubs.c
	@echo 'char* strdup(const char *s) { if(!s) return NULL; size_t len = strlen(s) + 1; char *p = malloc(len); if(p) memcpy(p, s, len); return p; }' >> stubs.c

# Create minimal vb9.h header
include/vb9.h:
	@mkdir -p include
	@echo "Creating minimal vb9.h header..."
	@echo '/* Minimal VB9 header for testing */' > include/vb9.h
	@echo '#ifndef VB9_H' >> include/vb9.h
	@echo '#define VB9_H' >> include/vb9.h
	@echo '#include <stdint.h>' >> include/vb9.h
	@echo '#include <stdarg.h>' >> include/vb9.h
	@echo '#include <stddef.h>' >> include/vb9.h
	@echo 'typedef struct { int x, y; } Point;' >> include/vb9.h
	@echo 'typedef struct VB9Form VB9Form;' >> include/vb9.h
	@echo 'typedef struct Control Control;' >> include/vb9.h
	@echo 'enum { VB9_BUTTON = 2, VB9_TEXTBOX = 3, VB9_LABEL = 5 };' >> include/vb9.h
	@echo 'void sysfatal(char *msg);' >> include/vb9.h
	@echo 'void* mallocz(size_t size, int zero);' >> include/vb9.h
	@echo 'void exits(char *msg);' >> include/vb9.h
	@echo 'int print(char *fmt, ...);' >> include/vb9.h
	@echo 'int snprint(char *buf, int len, char *fmt, ...);' >> include/vb9.h
	@echo 'char* strdup(const char *s);' >> include/vb9.h
	@echo '#define nil NULL' >> include/vb9.h
	@echo '#endif' >> include/vb9.h

test-vb9-advanced: $(COMMON_SRCS) $(TEST_SRCS) stubs.c include/vb9.h
	$(CC) $(CFLAGS) -o $@ $(COMMON_SRCS) $(TEST_SRCS) stubs.c $(LIBS)

clean:
	rm -f test-vb9-advanced glyph-kernel-standalone stubs.c include/vb9.h

.PHONY: all clean