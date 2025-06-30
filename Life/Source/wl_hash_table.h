#pragma once
#include "defs.h"
class wl_hash_table {
public:
  ptr_hash_table wl_new;
  int size;
  int used;
  ptr_keyword *data;
  wl_hash_table(int sz);

};
