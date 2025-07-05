/******************************* KEYWORDS *************************************
  RM: Feb 3 1993
  
  New version of the keyword table and related routines.

  The keyword table will not NOT be sorted, however, access will be hashed.

  Each module has its own hash table of symbols.

  All definition are stores in a linked list starting at first_definition.
*/
/* 	$Id: hash_table.c,v 1.2 1994/12/08 23:24:09 duchier Exp $	 */
#define EXTERN extern
#define REV401PLUS
#ifdef REV401PLUS
#include "defs.h"
#endif
/******** HASH_CREATE(size)
	  Create a hash-table for max size keywords.
*/
ptr_hash_table hash_create(int size)
{
  ptr_hash_table wl_new;
  int i;
  
  wl_new=(ptr_hash_table)malloc(sizeof(struct wl_hash_table));
  wl_new->size=size;
  wl_new->used=0;
  wl_new->data=(ptr_keyword *)malloc(size*sizeof(ptr_keyword));
  for(i=0;i<size;i++)
    wl_new->data[i]=NULL;
  return wl_new;
}

