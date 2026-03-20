#pragma once
#include "defs.h"
class wl_node_ptr {
public:
  ptr_node find(long long comp,char *keystr);
  ptr_node find_data(GENERIC p);
  ptr_psi_term make_feature_list(ptr_psi_term tail,
				 ptr_module module,int val);
  void mark_quote_tree_c(long long heap_flag);
  ptr_node copy_tree(long long copy_flag, long long heap_flag);
  ptr_node distinct_tree();
  void mark_eval_tree_new();
  void mark_quote_tree_new();
  void mark_quote_tree();
  void bk_mark_quote_tree();

  

};
