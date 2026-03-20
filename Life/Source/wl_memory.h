#pragma once
#include "defs.h"
#define LONELY 1
#define ALIGNUP(X) { (X) = (GENERIC)( ((long long) (X) + (ALIGN-1)) & ~(ALIGN-1) ); }
#define UNCHECKED(P) (! *((GENERIC)(P)+delta))

class wl_memory {
private:
  void pchoices();
   long long alloc_words;   
   long long mem_size;
   GENERIC mem_base;
   GENERIC heap_pointer;
   GENERIC mem_limit;
   GENERIC stack_pointer;
   float garbage_time;
   long long amount_used;
   long long pass;
#ifdef _WIN64
   clock_t last_garbage_time;
#endif
#ifdef __unix__
   struct tms last_garbage_time;
#endif
   float gc_time, life_time;
   long long delta;
   void print_undo_stack();
   long long bounds_undo_stack();
   void compress();
  long long unchecked(GENERIC *p, long long len);
  void fail_all();
  void check_string (GENERIC *s);
  void check_bytedata(GENERIC *s);
  void check_code(ptr_int_list *c);
  void check_pair_list(ptr_pair_list *p);
  void check_triple_list(ptr_triple_list *p);
  void check_kids(ptr_int_list *c);
  void check_operator_data(ptr_operator_data *op);
  void check_module_list(ptr_int_list *c);
  void check_module_tree(ptr_node *n);
  void check_module(ptr_module *m);
  void check_hash_table(ptr_hash_table table);
  void check_keyword(ptr_keyword *k);
  void check_definition_list();
  void check_def_code(ptr_definition *d);
  void check_def_rest(ptr_definition *d);
  void check_symbol(ptr_node *n);
  void check_type_disj(ptr_int_list *p);
  void check_goal_stack(ptr_goal *g);
  void check_resid(ptr_residuation *r);
  void check_resid_block(ptr_resid_block *rb);
  void check_psi_term(ptr_psi_term *t);
  void check_attr(ptr_node *n);
  void check_gamma_code();
  void check_gamma_rest();
  void check_undo_stack(ptr_stack *s);
  void check_choice_structs(ptr_choice_point *c);
  void check_choice(ptr_choice_point *c);
  void check_special_addresses();
  void check_psi_list(ptr_int_list *l);
  void check_resid_list(ptr_resid_list *l);
  void check_var(ptr_node *n);
  void check();
  void print_gc_info(long long timeflag);
public:
  void garbage();
  GENERIC heap_alloc (long long s);
  GENERIC stack_alloc(long long s);
  wl_memory ();
  void exit_mem_err_1();
  void exit_mem_err_2();
  long long bi_sys_t1();
  long long bi_sys_t2();
  long long bi_sys_t3();
  GENERIC heap_pointer_val();
  GENERIC stack_pointer_val();
  GENERIC mem_base_val();
  GENERIC mem_limit_val();
  float garbage_time_val();
  long long alloc_words_val();
  void set_stack_pointer(GENERIC val);
  void set_heap_pointer(GENERIC val);
  long long memory_check ();
  void check_definition(ptr_definition *d);
};
