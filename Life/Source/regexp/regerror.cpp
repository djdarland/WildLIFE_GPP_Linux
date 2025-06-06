#include <stdio.h>
#ifdef __unix__
#include <stdlib.h>
#endif
#ifdef _WIN64
#include <cstdlib>
#endif
#ifdef REG_NEEDED
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
