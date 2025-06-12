#pragma once
#include "defs.h"
class wl_node_ptr {
public:
  ptr_node find(long long comp,char *keystr);
  ptr_node find_data(GENERIC p);
};
