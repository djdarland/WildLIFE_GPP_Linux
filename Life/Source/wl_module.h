#pragma once
#include "defs.h"

class wl_module {
public:
  char *module_name;
  char *source_file;
  ptr_int_list open_modules;
  ptr_int_list inherited_modules;
  ptr_hash_table symbol_table;
};


typedef class wl_module * ptr_module;

