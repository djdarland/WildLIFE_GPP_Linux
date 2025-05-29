#include <stdio.h>
#include <stdlib.h>
#ifdef NEEDED
void regerror(char *s)
// char *s;
{
#ifdef ERRAVAIL
	error("regexp: %s", s);
#else
	fprintf(stderr, "regexp(3): %s", s);
	exit(1);
#endif
	/* NOTREACHED */
}
#endif