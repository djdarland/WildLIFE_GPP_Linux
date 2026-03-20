#define EXTERN extern
#include "defs.h"
wl_hash_table::wl_hash_table(int sz)
{
  int i;
  
  wl_new=(ptr_hash_table)malloc(sizeof(struct wl_hash_table));
  wl_new->size=sz;
  wl_new->used=0;
  wl_new->data=(ptr_keyword *)malloc(size*sizeof(ptr_keyword));
  for(i=0;i<size;i++)
    wl_new->data[i]=NULL;
  // return wl_new;
}

