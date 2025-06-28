#define EXTERN extern
#include "defs.h" 

wl_buckets::wl_buckets()
/******** INIT_COPY()
	  Execute once upon startup of Wild_Life.
*/
{
  long long i;

  for(i=0; i<HASHSIZE; i++) hashtable[i].timestamp = 0;
  hashtime = 0;
  numbuckets = NUMBUCKETS;
  hashbuckets = (struct hashbucket *)
    malloc(NUMBUCKETS * sizeof(struct hashbucket));
}
/******** CLEAR_COPY()
	  Erase the hash table.
	  This must be done as a prelude to any copying operation.
*/
void wl_buckets::clear_copy()
{
  hashtime++;
  hashfree=0;
}

