/* Copyright 1991 Digital Equipment Corporation.
** All Rights Reserved.
*****************************************************************/
/* 	$Id: trees.c,v 1.3 1995/07/27 21:22:21 duchier Exp $	 */
#define EXTERN extern
#define REV401PLUS
#ifdef REV401PLUS
#include "defs.h"
#endif
/******** INTCMP(a,b)
	  Compares two integers, for use in FIND or INSERT.
*/
long long intcmp(long long a,long long b)
// long long a;
// long long b;
{
  return a-b;
}
/* Return TRUE iff the string s represents an integer. */
/* Modify s to point to first non-zero digit. */
/* Return number of significant digits in the integer and its sign. */
long long is_int(char **s, long long *len, long long *sgn)
// char **s;
// long long *len;
// long long *sgn;
{
  char *sint; /* Ptr to first non-zero digit */
  char *stmp; /* Scratchpad for string ptr */
  stmp=(*s);
  if (*sgn=(*stmp=='-')) {
    stmp++;
    if (!*stmp) return FALSE;
  }
  if (!*stmp) return FALSE; /* 6.10 */
  while (*stmp=='0') stmp++;
  sint=stmp;
  while (*stmp) {
    if (*stmp<'0' || *stmp>'9') return FALSE;
    stmp++;
  }
  *len=stmp-sint;
  *s=sint;
  return TRUE;
}
/******** FEATCMP(s1,s2)
	  Compares two strings which represent features, for use
	  in FIND or INSERT.  This differs from strcmp for those strings
	  that represent integers.  These are compared as integers.
	  In addition, all integers are considered to be less than
	  all strings that do not represent integers.
*/
long long featcmp(char *str1, char *str2)
// char *str1, *str2;
{
  long long len1,len2,sgn1,sgn2;
  char *s1,*s2;

  if(str1==str2)
    return 0;
  if(*(str1+1)==0 && *(str2+1)==0)
    return *str1 - *str2;
  s1=str1; /* Local copies of the pointers */
  s2=str2;
  if (is_int(&s1,&len1,&sgn1)) {
    if (is_int(&s2,&len2,&sgn2)) {
      if (sgn1!=sgn2) return (sgn2-sgn1); /* Check signs first */
      if (len1!=len2) return (len1-len2); /* Then check lengths */
      return strcmp(s1,s2); /* Use strcmp only if same sign and length */
    }
    else
      return -1;
  }
  else {
    if (is_int(&s2,&len2,&sgn2))
      return 1;
    else
      return strcmp(s1,s2);
  }
}
/******** HEAP_NCOPY_STRING(string,length)
	  Make a copy of the string in the heap, and return a pointer to that.
	  Exceptions: "1" and "2" are unique (and in the heap).
*/
char *heap_ncopy_string(char *s,int n)
// char *s;
// int n;
{
  char *p;
  
  if (s==one || s==two) return s;
  p=(char *)wl_mem->heap_alloc(n+1);
  strncpy(p,s,n);
  p[n]='\0';
  return p;
}
/******** HEAP_COPY_STRING(string)
	  Make a copy of the string in the heap, and return a pointer to that.
	  Exceptions: "1" and "2" are unique (and in the heap).
*/
char *heap_copy_string(char *s)
// char *s;
{ return heap_ncopy_string(s,strlen(s)); }
/******** STACK_COPY_STRING(string)
	  Make a copy of the string in the stack, and return a pointer to that.
	  Exceptions: "1" and "2" are unique (and in the heap).
*/
char *stack_copy_string(char *s)
// char *s;
{
  char *p;
  if (s==one || s==two) return s;
  p=(char *)wl_mem->stack_alloc(strlen(s)+1);
  strcpy(p,s);
  return p;
}




