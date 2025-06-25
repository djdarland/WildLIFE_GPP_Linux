#define EXTERN extern
#include "defs.h"
/********************************************************************/
/*                                                                  */
/* New dereference routines for Wild_Life                           */
/* These routines handle evaluation-by-need.  Check_out is changed  */
/* to no long longer call check_func, which is done in the new routines. */
/* Functions inside of psi-terms are only evaluated if needed.  It  */
/* is assumed that 'needed' is true when they are derefed.          */
/*                                                                  */
/* There are three new dereference routines:                        */
/*    deref_eval(P)                                                 */
/*       If the psi-term P is a function, call check_func to        */
/*       push eval goals so that the function will be evaluated.    */
/*       Then return TRUE so that the caller can itself return.     */
/*       This only looks at the top level.                          */
/*    deref_rec(P)                                                  */
/*       If the psi-term P recursively contains any functions, then */
/*       push eval goals to evaluate all of them.  Set a global     */
/*       variable deref_flag if this is the case.                   */
/*    deref_args(P,S)                                               */
/*       Same as above, except does not look at the top level or at */
/*       the arguments named in the set S.                          */
/*       This is needed to guarantee evaluation of all arguments of */
/*       a built-in, even those not used by the built-in.           */
/*                                                                  */
/* The original dereference macro is renamed to:                    */
/*    deref_ptr(P) = while (P->coref) P=P->coref                    */
/* There are three new macros:                                      */
/*    deref(P)        = deref_ptr(P);                               */
/*                      if (deref_eval(P)) then return TRUE         */
/*    deref_rec(P)    = deref_ptr(P);                               */
/*                      if (deref_rec_eval(P)) then return TRUE     */
/*    deref_args(P,S) = deref_ptr(P);                               */
/*                      if (deref_args_eval(P,S)) then return TRUE  */
/*                                                                  */
/********************************************************************/
static long long deref_flag;
// void deref_rec_body();
// void deref_rec_args();
// void deref_rec_args_exc();
/* Ensure evaluation of top of psi-term */
long long wl_psi_term_ptr::deref_eval()
// ptr_psi_term t;
{
  ptr_psi_term t;
  t = (ptr_psi_term) this;
  ptr_goal save=goal_stack;

  deref_flag=FALSE;
  goal_stack=aim;
  if (t->status==0) {
    if(t->type->type_def==(def_type)function_it) {
      check_func(t);    /* Push eval goals to evaluate the function. */
      deref_flag=TRUE;  /* TRUE so that caller will return to main_prove. */
    }
    else
      if(t->type->type_def==(def_type)global_it) { /*  RM: Feb 10 1993  */
	eval_global_var(t);
	deref_ptr(t);/*  RM: Jun 25 1993  */
	deref_flag=((wl_psi_term_ptr*)t)->deref_eval();
      }
      else {
	if (t->status!=2) {
	  if((GENERIC)t<wl_mem->heap_pointer_val())
	    push_ptr_value(int_ptr,(GENERIC *)&(t->status)); /*  RM: Jul 15 1993  */ // REV401PLUS cast
	  t->status=4;
	  deref_flag=FALSE;
	}
      }
  }
  else
    deref_flag=FALSE;
  if (!deref_flag) goal_stack=save;
  return (deref_flag);
}
/* Ensure evaluation of *all* of psi-term */
long long wl_psi_term_ptr::deref_rec_eval()
// ptr_psi_term t;
{
  ptr_psi_term t;
  t = (ptr_psi_term) this;
  ptr_goal save=goal_stack;

  deref_flag=FALSE;
  goal_stack=aim;
  ((wl_psi_term_ptr*)t)->deref_rec_body();
  if (!deref_flag) goal_stack=save;
  return (deref_flag);
}
void wl_psi_term_ptr::deref_rec_body()
// ptr_psi_term t;
{
  ptr_psi_term t;
  t = (ptr_psi_term) this;
  if (t->status==0) {
    if (t->type->type_def==(def_type)function_it) {
      check_func(t);
      deref_flag=TRUE;
    }
    else
      if(t->type->type_def==(def_type)global_it) { /*  RM: Feb 10 1993  */
	eval_global_var(t);
	deref_ptr(t);/*  RM: Jun 25 1993  */
	((wl_psi_term_ptr*)t)->deref_rec_body();
      }
      else {
	if((GENERIC)t<wl_mem->heap_pointer_val())
	  push_ptr_value(int_ptr,(GENERIC *)&(t->status));/*  RM: Jul 15 1993  */ // REV401PLUS cast
	t->status=4;
	deref_rec_args(t->attr_list);
      }
  }
}
/* Same as deref_rec_eval, but doesn't look at either the top level or */
/* the arguments in the set. */
long long wl_psi_term_ptr::deref_args_eval(long long set)
// ptr_psi_term t;
// long long set;
{
  ptr_psi_term t;
  t = (ptr_psi_term) this;
  ptr_goal save = goal_stack;
  ptr_goal top = aim;

  deref_flag = FALSE;
  goal_stack = top;
  deref_rec_args_exc(t->attr_list,set);
  if (!deref_flag) goal_stack = save;
  return (deref_flag);
}
/* These two needed only for match_aim and match_attr: */
/* Same as deref_eval, but assumes goal_stack already restored. */
void wl_psi_term_ptr::deref2_eval()
//ptr_psi_term t;
{
  ptr_psi_term t;
  t = (ptr_psi_term) this;
  deref_ptr(t);
  if (t->status==0) {
    if (t->type->type_def==(def_type)function_it) {
      check_func(t);
    }
    else 
      if(t->type->type_def==(def_type)global_it) { /*  RM: Feb 10 1993  */
      	eval_global_var(t);
	deref_ptr(t);/*  RM: Jun 25 1993  */
	((wl_psi_term_ptr*)t)->deref2_eval();
      }
      else {
	t->status=4;
      }
  }
}
/* Same as deref_rec_eval, but assumes goal_stack already restored. */
void wl_psi_term_ptr::deref2_rec_eval()
// ptr_psi_term t;
{
  ptr_psi_term t;
  t = (ptr_psi_term) this;
  deref_ptr(t);
  ((wl_psi_term_ptr*)t)->deref_rec_body();
}
