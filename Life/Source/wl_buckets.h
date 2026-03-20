#pragma once
#include "defs.h"

/****************************************************************************/

/* New translation routines for Wild_Life                     */
/* These routines work for any size structure.                */
/* They are based on a hash table with buckets and timestamp. */
/* This allows fast clearing of the table and fast insertion  */
/* and lookup.                                                */

/* Size of hash table; must be a power of 2 */
/* A big hash table means it is sparse and therefore fast */
class wl_buckets {
public:
  struct hashentry hashtable[HASHSIZE];
  struct hashbucket *hashbuckets; /* Array of buckets */
  long long hashtime; /* Currently valid timestamp */
  long long hashfree; /* Index into array of buckets */
  long long numbuckets; /* Total number of buckets; initially=NUMBUCKETS */
  
  wl_buckets();
  void clear_copy();
  friend class wl_psi_term_ptr;
};
