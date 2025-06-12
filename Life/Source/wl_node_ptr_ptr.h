#pragma once
#include "defs.h"
class wl_node_ptr_ptr {
public:
  ptr_node general_insert(long long comp,char *keystr,
			  GENERIC info,
			  long long heapflag, long long copystr,
			  long long bkflag);
  void heap_insert_copystr(char *keystr, GENERIC info);
  void stack_insert_copystr(char *keystr,GENERIC info);
  ptr_node heap_insert(long long comp,char *keystr,GENERIC info);
  ptr_node stack_insert(long long comp,char *keystr,GENERIC info);
  ptr_node bk_stack_insert(long long comp,char *keystr,GENERIC info);
  ptr_node bk2_stack_insert(long long comp,char *keystr,GENERIC info);
  void delete_attr(char *s);
};
