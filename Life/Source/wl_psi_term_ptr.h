#pragma once
#include "defs.h"
class wl_psi_term_ptr {
public:
  long long deref_eval();
  long long deref_rec_eval();
  void deref_rec_body();
  long long deref_args_eval(long long set);
  void deref2_eval();
  void deref2_rec_eval();
  ptr_goal makeGoal();

};
