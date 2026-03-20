#pragma once
#include "defs.h"
class wl_module_ptr {
public:
  //  static long long built_in_index=0;

  ptr_definition update_symbol(char *symbol);

  ptr_definition update_feature(char *feature);
  
  void new_built_in(char *s,def_type t,long long (*r)());
  char *make_module_token(char *str);

};
