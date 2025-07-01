#define EXTERN extern
#include "defs.h"
char *wl_node::key_val()
{
  return key;
}
void wl_node::set_key(char *val)
{
  key = val;
}
ptr_node wl_node::left_val()
{
  return left;
}
void wl_node::set_left(ptr_node val)
{
  left = val;
}
ptr_node wl_node::right_val()
{
  return right;
}
void wl_node::set_right(ptr_node val)
{
  right = val;
}
GENERIC wl_node::data_val()
{
  return data;
}
void wl_node::set_data(GENERIC val)
{
  data = val;
}
GENERIC *wl_node::data_addr()
{
  return(&data);
}

ptr_node *wl_node::right_addr()
{
  return(&right);
}

ptr_node *wl_node::left_addr()
{
  return(&left);
}

