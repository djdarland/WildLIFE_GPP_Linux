#pragma once
#include "defs.h"

class wl_node {
  friend class wl_node_ptr;
  friend class wl_node_ptr_ptr;
  friend class wl_memory;
public:
  char *key_val();
  void set_key(char *val);
  ptr_node left_val();
  void set_left(ptr_node val);
  ptr_node right_val();
  void set_right(ptr_node val);
  GENERIC data_val();
  void set_data(GENERIC val);
  GENERIC *data_addr();
  ptr_node *right_addr();
  ptr_node *left_addr();

private:
  char *key;
  ptr_node left;
  ptr_node right;
  GENERIC data;
  friend class wl_psi_ptr;
};
