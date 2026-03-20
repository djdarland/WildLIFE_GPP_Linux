#pragma once
#include "defs.h"
class wl_hash_table_ptr {
public:
  void hash_expand(int new_size);
  int hash_code(char *symbol);
  int hash_find(char *symbol);
  ptr_keyword hash_lookup(char *symbol);
  void hash_insert(char *symbol,ptr_keyword keyword);
  void hash_display();
};
