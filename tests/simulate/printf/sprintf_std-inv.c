/* Test of sprintf_std(), invalid format.
   $Id$	*/

#ifndef __AVR__
# define PRINTFLN(line, fmt, ...)	\
    printf("\nLine %2d: " fmt "\n", line, ##__VA_ARGS__)
# define EXIT(code)	exit ((code) < 255 ? (code) : 255)
# define sprintf_P	sprintf
#else
# define PRINTFLN(args...)
# define EXIT	exit
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "progmem.h"

void Check (int line,
	    int explen,
            const char *expstr,
	    int retval, const char *retstr)
{
    int code;

    if (retval != explen)
	code = 1000 + line;
    else if (strcmp_P (retstr, expstr))
	code = line;
    else
	return;
    PRINTFLN (line, "expect: %3d, \"%s\",\n%8s output: %3d, \"%s\"\n",
	      explen, expstr, " ", retval, retstr);
#ifdef	DEBUG
    code = (int)retstr;
#endif
    EXIT (code);
}

/* 'vp' is used to avoid gcc warnings about format string.	*/
#define CHECK(explen, expstr, fmt, ...)	do {			\
    char s[260];						\
    int i;							\
    int (* volatile vp)(char *, const char *, ...) = sprintf_P;	\
    memset (s, 0, sizeof(s));					\
    i = vp (s, PSTR(fmt), ##__VA_ARGS__);			\
    Check (__LINE__, explen, PSTR(expstr), i, s);		\
} while (0)

int main ()
{
#ifdef	__AVR__		/* PRINTF_STD */
    /* Float numbers: are skipped	*/
    CHECK (1, "?", "%e", 0.0);
    CHECK (23, "? e ? f ? g ? E ? F ? G",
	   "%e %c %e %c %e %c %e %c %e %c %e %c",
	   1.0, 'e', 2.0, 'f', 3.0, 'g', 4.0, 'E', 5.0, 'F', 6.0, 'G');
    CHECK (19, "? 10 ? 11 ? 12 ? 13",
	   "%+e %d %-f %d % g %d %0F %d",
	   10.0, 10, 11.0, 11, 12.0, 12, 13.0, 13);

    /* Width is work.	*/
    CHECK (17, " ?.  ?.   ?.    ?",
	   "%2e.%3.1f.%+4g.% 5E", 1.0, 2.0, 3.0, 4.0);

    /* Left pad is work.	*/
    CHECK (6, "? .?  ", "%-2F.%-03G", 5.0, 6.0);
#endif

    return 0;
}