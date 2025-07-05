#pragma once
#include "defs.h"
class wl_node_ptr_ptr {
public:
  static long long attr_missing;
  static long long check_func_flag;
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
  void match_attr1(ptr_node v,ptr_resid_block rb);
  void match_attr2(ptr_node v,ptr_resid_block rb);
  void match_attr3(ptr_node v,ptr_resid_block rb);
  void match_attr(ptr_node v,ptr_resid_block rb);



  
};
