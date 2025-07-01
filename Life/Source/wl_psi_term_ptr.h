#pragma once
#include "defs.h"
class wl_psi_term_ptr {
public:
  static long long curr_status;
  static long long mark_nonstrict_flag;
  long long deref_eval();
  long long deref_rec_eval();
  void deref_rec_body();
  long long deref_args_eval(long long set);
  void deref2_eval();
  void deref2_rec_eval();
  ptr_goal makeGoal();
  long long isa(ptr_psi_term arg2);
  long long isa_select(ptr_psi_term arg2, long long sel);
  long long isValue();
  ptr_psi_term stack_cons(ptr_psi_term tail);
  ptr_psi_term stack_pair(ptr_psi_term right);
  void insert_translation(ptr_psi_term b,long long info);
  ptr_psi_term translate(long long **infoptr);
  void mark_quote_c(long long heap_flag);
  ptr_psi_term exact_copy(long long heap_flag);
  ptr_psi_term quote_copy(long long heap_flag);
  ptr_psi_term eval_copy(long long heap_flag);
  ptr_psi_term inc_heap_copy();
  ptr_psi_term copy(long long copy_flag, long long heap_flag);
  ptr_psi_term distinct_copy();
  //////////
  void mark_eval();
  void mark_nonstrict();
  void mark_quote_new2();
  void mark_eval_new();
  void mark_quote_new();
  void mark_quote();
  void bk_mark_quote();


};
