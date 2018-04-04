#include <stdio.h>
#include <stdarg.h>
/**
 * print an error message and die
 */
void error ( char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
        fprintf(stderr,"Run-time error...\n");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
        fprintf(stderr,"...now exiting to system...\n");
	va_end(args);
	exit(1);
}
