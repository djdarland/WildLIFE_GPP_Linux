/* Copyright 1991 Digital Equipment Corporation.
** All Rights Reserved.
*****************************************************************/
/* 	$Id: lefun.c,v 1.4 1995/01/14 00:24:55 duchier Exp $	 */
#define EXTERN extern
#define REV401PLUS
#ifdef REV401PLUS
#include "defs.h"
#endif
// static long long attr_missing;
// static long long check_func_flag;
/* Create a new psi_term on the stack with value '@' (top) and no attributes. */
ptr_psi_term stack_psi_term(long long stat)
// long long stat;
{
  ptr_psi_term result;

  result=STACK_ALLOC(psi_term);
  result->type=top;
  result->status=stat;
  result->flags=stat?QUOTED_TRUE:FALSE; /* 14.9 */
  result->attr_list=NULL;
  result->coref=NULL;
#ifdef TS
  result->time_stamp=global_time_stamp; /* 9.6 */
#endif
  result->resid=NULL;
  result->value_3=NULL;
  return result;
}
/* Create a new psi_term on the stack with a real number value. */
ptr_psi_term real_stack_psi_term(long long stat,REAL thereal)
// long long stat;
// REAL thereal;
{
  ptr_psi_term result;

  result=STACK_ALLOC(psi_term);
  result->type = (thereal==floor(thereal)) ? integer : real;
  result->status=stat;
  result->flags=stat?QUOTED_TRUE:FALSE; /* 14.9 */
  result->attr_list=NULL;
  result->coref=NULL;
#ifdef TS
  result->time_stamp=global_time_stamp; /* 9.6 */
#endif
  result->resid=NULL;
  result->value_3=(GENERIC)wl_mem->heap_alloc(sizeof(REAL));
  (* (REAL *)(result->value_3)) = thereal;

  return result;
}
/* Create a new psi_term on the heap with value '@' (top) and no attributes. */
ptr_psi_term heap_psi_term(long long stat)
// long long stat;
{
  ptr_psi_term result;

  result=HEAP_ALLOC(psi_term);
  result->type=top;
  result->status=stat;
  result->flags=stat?QUOTED_TRUE:FALSE; /* 14.9 */
  result->attr_list=NULL;
  result->coref=NULL;
#ifdef TS
  result->time_stamp=global_time_stamp; /* 9.6 */
#endif
  result->resid=NULL;
  result->value_3=NULL;

  return result;
}

/******** CURRY()
Decide that the current function will have to be curried.
This has become so simple it could be a MACRO.
The real work is done by DO_CURRY.
*/
void curry()
{
  if (can_curry)
    curried=TRUE;
}
/******** RESIDUATEGOALONVAR(g,var,othervar)
Add the goal to the variable's residuation list.
Also update the residuation's 'bestsort' field if it exists (needed to
implement complete disequality semantics).  The 'othervar' parameter
is needed for this.
*/
long long residuateGoalOnVar(ptr_goal g, ptr_psi_term var, ptr_psi_term othervar)
// ptr_goal g;
// ptr_psi_term var,othervar;
{
  long long result;
  long long resflag,resflag2;
  GENERIC rescode,rescode2,resvalue,resvalue2;
  /* Set to FALSE if the goal is already residuated on the var: */
  long long not_found = TRUE;
  /* Points to a pointer to a residuation structure.  Used so we can */
  /* add the goal to the end of the residuation list, so that it can */
  /* can be undone later if backtracking happens.  See the call to   */
  /* push_ptr_value.  */
  ptr_residuation *r;
    
  /* 5.8 PVR */
  if ((GENERIC)var>=wl_mem->heap_pointer_val()) {
    Errorline("attempt to residuate on psi-term %P in the heap.\n",var);
    return FALSE;
  }
  r= &(var->resid);
  while (not_found && *r) {
    if ((*r)->goal == g) { /* This goal is already attached */
      /* Keep track of best sort so far */
      /* Glb_code(..) tries to keep 'sortflag' TRUE if possible. */
      result=glb_code((*r)->sortflag,(*r)->bestsort,
		      TRUE,(GENERIC)var->type,&resflag,&rescode); //REV401PLUS cast
      result=glb_value(result,resflag,rescode,(*r)->value_2,var->value_3,
		       &resvalue); /* 6.10 */
      if (!result)
        return FALSE; /* 21.9 */
      else if (othervar) {
	result=glb_code(resflag,rescode,TRUE,(GENERIC)othervar->type, //REV401PLUS cast
			&resflag2,&rescode2);
        result=glb_value(result,resflag2,rescode2,resvalue,othervar->value_3,
			 &resvalue2); /* 6.10 */
        if (!result) {
          return FALSE;
        }
        else {
	  /* The value field only has to be trailed once, since its value */
	  /* does not change, once given. */
	  if ((*r)->value_2==NULL && resvalue2!=NULL) { /* 6.10 */
	    push_ptr_value(int_ptr,&((*r)->value_2));
	  }
	  if ((*r)->bestsort!=rescode2) {
            push_ptr_value(((*r)->sortflag?def_ptr:code_ptr),
			   &((*r)->bestsort));
            (*r)->bestsort=rescode2; /* 21.9 */
	  }
	  if ((*r)->sortflag!=resflag2) {
            push_ptr_value(int_ptr,(GENERIC *)&((*r)->sortflag)); //REV401PLUS cast
            (*r)->sortflag=resflag2; /* 21.9 */
	  }
	}
      }
      else {
	if ((*r)->value_2==NULL && resvalue!=NULL) { /* 6.10 */
	  push_ptr_value(int_ptr,&((*r)->value_2));
	}
	if ((*r)->bestsort!=rescode) {
          push_ptr_value(((*r)->sortflag?def_ptr:code_ptr),
	                 &((*r)->bestsort));
          (*r)->bestsort=rescode; /* 21.9 */
	}
	if ((*r)->sortflag!=resflag) {
          push_ptr_value(int_ptr,(GENERIC *)&((*r)->sortflag)); //REV401PLUS cast
          (*r)->sortflag=resflag; /* 21.9 */
	}
      }
      not_found = FALSE;
    }
    else
      r= &((*r)->next);  /* look at the next one */
  }
  
  if (not_found) {
    /* We must attach this goal & the variable's sort onto this variable */
    push_ptr_value(resid_ptr,(GENERIC *)r); // REV401PLUS cast
    *r=STACK_ALLOC(residuation);
    if (othervar) {
      result=glb_code(TRUE,(GENERIC)var->type,TRUE,(GENERIC)othervar->type,&resflag,&rescode); // REV401PLUS cast
      result=glb_value(result,resflag,rescode,var->value_3,othervar->value_3,
		       &resvalue); /* 6.10 */ // REV401PLUS casts
      if (!result) {
        return FALSE;
      }
      else {
	(*r)->sortflag=resflag;
        (*r)->bestsort=rescode; /* 21.9 */
	(*r)->value_2=resvalue; /* 6.10 */  
      }
    }
    else {
      (*r)->sortflag=TRUE;
      (*r)->bestsort=(GENERIC)var->type; /* 21.9 */
      (*r)->value_2=(GENERIC)var->value_3; /* 6.10 */
    }
    (*r)->goal=g;
    (*r)->next=NULL;
  }
  if (!(g->pending)) {
    /* this goal is not pending, so make sure it will be put on the goal
     * stack later
     */
    push_ptr_value(int_ptr,(GENERIC *)&(g->pending)); // REV401PLUS cast
    g->pending=(ptr_definition)TRUE; // REV401PLUS cast
  }
  return TRUE; /* 21.9 */
}
/******** DO_RESIDUATION()
Undo anything that matching may have done, then
create a residuated expression. Check that the same constraint does not
hang several times on the same variable.

This routine takes time proportional to the square of the number of
residuations.  This is too slow; eventually it should be sped up, 
especially if equality constraints are often used.
*/
long long do_residuation(); /* forward declaration */

/* LIFE-defined routines reset the goal stack to what it was */
/* before the function call. */
long long do_residuation_user()
{
  goal_stack=resid_aim->next; /* reset goal stack */
  return do_residuation();
}
/* C-defined routines do all stack manipulation themselves */
long long do_residuation()
{
  long long success;
  ptr_psi_term t,u;
  ptr_goal *gs;
  
  /* This undoes perfectly valid work! */
  /* The old version of Wild_Life did not trail anything
     during matching, so I think this was a nop for it. */
  /* PVR 11.5 undo(resid_limit); */
  /* PVR 11.5 choice_stack=cut_point; */

  /* PVR 9.2.94 */
  /* goal_stack=resid_aim->next; */
  if (trace) {
    tracing();
    print_resid_message(resid_aim->aaaa_1,resid_vars);
  }
  while (resid_vars) {
    t=resid_vars->var; /* 21.9 */
    u=resid_vars->othervar; /* 21.9 */
    /* PVR */ deref_ptr(t);
    resid_vars=resid_vars->next;
    Traceline("residuating on %P (other = %P)\n",t,u);
    success=residuateGoalOnVar(resid_aim, t, u); /* 21.9 */
    if (!success) { /* 21.9 */
      Traceline("failure because of disentailment\n");
      return FALSE;
    }
  }
  Traceline("no failure because of disentailment\n");
  return TRUE; /* 21.9 */
}
/********* DO_CURRYING()
This performs CURRYing: all that needs to be done is to yield the calling
term as the result after having given up on evaluation. In effect the calling
psi-term is left intact.
*/
void do_currying()
{
  ptr_psi_term funct,result;

  goal_stack=resid_aim->next;
  funct=(ptr_psi_term )resid_aim->aaaa_1;
  result=(ptr_psi_term )resid_aim->bbbb_1;
  Traceline("currying %P\n",funct);
  push_goal(unify_noeval,funct,result,NULL);
  resid_aim=NULL;
}
/******** EVAL_AIM()
Evaluate a function.
This copies the current definition of the function and
stacking the various goals that are necessary to evaluate the function
correctly.
It creates an extra psi-term (with value top) in which to write the result.
*/
long long eval_aim()
{
  long long success=TRUE;
  ptr_psi_term funct,result,head,body;
  ptr_pair_list rule;
  /* RESID */ ptr_resid_block rb;
  ptr_choice_point cutpt;
  ptr_psi_term match_date; /* 13.6 */
  
  funct=(ptr_psi_term )aim->aaaa_1;
  deref_ptr(funct);

  /*  RM: Jun 18 1993  */
  push2_ptr_value(int_ptr,(GENERIC *)&(funct->status),(GENERIC)(funct->status & SMASK)); // REV401PLUS cast
  funct->status=4;
  result=(ptr_psi_term )aim->bbbb_1;
  rule=(ptr_pair_list )aim->cccc_1;
  match_date=(ptr_psi_term )wl_mem->stack_pointer_val();
  cutpt=choice_stack; /* 13.6 */
  /* For currying and residuation */
  curried=FALSE;
  can_curry=TRUE;
  resid_vars=NULL;
  if (rule) {
    Traceline("evaluate %P\n",funct);
    if ((unsigned long long)rule<=MAX_BUILT_INS) {
      resid_aim=aim;
      success=c_rule[(unsigned long long)rule]();
      if (curried)
	do_currying();
      else
	if (resid_vars)
	  success=do_residuation(); /* 21.9 */
	else {
        }
    }
    else {
      while (rule && (rule->aaaa_2==NULL || rule->bbbb_2==NULL)) {
        rule=rule->next;
        Traceline("alternative rule has been retracted\n");
      }
      if (rule) {
        resid_aim=aim;
        /* RESID */ rb = STACK_ALLOC(resid_block);
        /* RESID */ save_resid(rb,match_date);
        /* RESID */ /* resid_aim = NULL; */
        wl_bucks->clear_copy();
	/*  RM: Jun 18 1993: no functions in head */
	if (rule->aaaa_2)head=((wl_psi_term_ptr*)rule->aaaa_2)->quote_copy(STACK); else head = NULL;
        if (rule->bbbb_2)body=((wl_psi_term_ptr*)rule->bbbb_2)->eval_copy(STACK); else body = NULL;
	head->status=4;
        if (rule->next) /* 17.6 */
          push_choice_point(eval,funct,result,(GENERIC)rule->next); // REV401PLUS cast
        push_goal(unify,body,result,NULL);
        /* RESID */ push_goal(eval_cut,body,(ptr_psi_term)cutpt,(GENERIC)rb); /* 13.6 */ // REV401PLUS casts
        /* RESID */ push_goal(match,funct,head,(GENERIC)rb); // REV401PLUS cast
      }
      else {
        success=FALSE;
      }
    }
  }
  else {
    success=FALSE;
  }
  resid_aim=NULL;
  return success;
}
/******** MATCH_AIM()
This is very similar to UNIFY_AIM, only matching cannot modify the calling
psi_term.   The first argument is the calling term (which may not be changed)
and the second argument is the function definition (which may be changed).
Residuate the expression if the calling term is more general than the
function definition.
*/
long long match_aim()
{
  long long success=TRUE;
  ptr_psi_term u,v,tmp;
  REAL r;
  long long less,lesseq;
  ptr_resid_block rb;
  ptr_psi_term match_date;
  
  u=(ptr_psi_term )aim->aaaa_1;
  v=(ptr_psi_term )aim->bbbb_1;
  deref_ptr(u);
  deref_ptr(v);
  rb=(ptr_resid_block)aim->cccc_1;
  restore_resid(rb,&match_date);
  if (u!=v) {
    if (success=matches(u->type,v->type,&lesseq)) {
      if (lesseq) {
        if (u->type!=cut || v->type!=cut) { /* Ignore value field for cut! */
          if (v->value_3) {
            if (u->value_3) {
              if (overlap_type(v->type,real))
                success=(*((REAL *)u->value_3)==(*((REAL *)v->value_3)));
              else if (overlap_type(v->type,quoted_string))
                success=(strcmp((char *)u->value_3,(char *)v->value_3)==0);
	      /* DENYS: BYTEDATA */
              else if (overlap_type(v->type,sys_bytedata)) {
		unsigned long long ulen = *((unsigned long long *) u->value_3);
		unsigned long long vlen = *((unsigned long long *) v->value_3);
                success=(ulen==vlen && bcmp((char *)u->value_3,(char *)v->value_3,ulen)==0);
	      }
            }
            else
              ((wl_psi_term_ptr*)u)->residuate_double(v);
          }
        }
      }
      else if (u->value_3) {
        /* Here we have U <| V but U and V have values which cannot match. */
        success=TRUE;
        if (v->value_3) {
          if (overlap_type(v->type,real))
            success=(*((REAL *)u->value_3)==(*((REAL *)v->value_3)));
        }
        else if (overlap_type(u->type,integer)) {
          r= *((REAL *)u->value_3);
          success=(r==floor(r));
        }
        if (success) ((wl_psi_term_ptr*)u)->residuate_double(v);
      } 
      else
        ((wl_psi_term_ptr*)u)->residuate_double(v);
      if (success) {
        if (FUNC_ARG(u) && FUNC_ARG(v)) { /*  RM: Feb 10 1993  */
          /* residuate2(u,v); 21.9 */
          ((wl_psi_term_ptr*)u)->residuate_double(v); /* 21.9 */
          ((wl_psi_term_ptr*)v)->residuate_double(u); /* 21.9 */
	}
        else if (FUNC_ARG(v)) {  /*  RM: Feb 10 1993  */
          ((wl_psi_term_ptr*)v)->residuate_double(u); /* 21.9 */
        }
        else {
          v->coref=u;
        } /* 21.9 */
	wl_node_ptr_ptr::attr_missing=FALSE;
	((wl_node_ptr_ptr*)&(u->attr_list))->match_attr(v->attr_list,rb);
	if (wl_node_ptr_ptr::attr_missing) {
	  if (can_curry)
	    curried=TRUE;
	  else
	    ((wl_psi_term_ptr*)u)->residuate_double(v);
	}
        /* } 21.9 */
      }
    }
  }
  can_curry=FALSE;
  save_resid(rb,match_date); /* updated resid_block */
  /* This should be a useless statement: */
  resid_aim = NULL;
  return success;
}
/******************************************************************************
  The following routines prepare terms for unification, proof or matching.
  They deal with conjunctions, disjunctions, functions and arguments which
  have to be examined before the general proof can continue.
*/
/* Forward declarations */
long long check_out();
long long eval_args();
/******** EVAL_ARGS(n)
	  N is an attribute tree, the attributes must be examined, if any reveal
	  themselves to need evaluating then return FALSE.
*/
long long i_eval_args(ptr_node n)
// ptr_node n;
{
  wl_node_ptr_ptr::check_func_flag=FALSE;
  return eval_args(n);
}
long long eval_args(ptr_node n)
// ptr_node n;
{
  long long flag=TRUE;
  
  if (n) {
    flag = eval_args(n->right_val());
    flag = check_out((ptr_psi_term)n->data_val()) && flag; // REV401PLUS cast
    flag = eval_args(n->left_val()) && flag;
  }
  return flag;
}
/******** CHECK_DISJ(t)
	  Deal with disjunctions.
*/
void check_disj(ptr_psi_term t)
// ptr_psi_term t;
{
  Traceline("push disjunction goal %P\n",t);
  if (t->value_3) 
    push_goal(disj,t,t,(GENERIC)TRUE); /* 18.2 PVR */
  else
    push_goal(fail,NULL,NULL,NULL);
}
/******** CHECK_FUNC(t)
Deal with an unevaluated function: push an 'eval' goal for it, which will
cause it to be evaluated.
*/
void check_func(ptr_psi_term t)
//ptr_psi_term t;
{
  ptr_psi_term result,t1,copy;

 {
    Traceline("setting up function call %P\n",t);
    /* Create a psi-term to put the result */
    result = stack_psi_term(0);
    /* Make a partial copy of the calling term */
    copy=stack_copy_psi_term(*t);
    copy->status &= ~RMASK;
    /* Bind the calling term to the result */
    push_psi_ptr_value(t,(GENERIC *)&(t->coref)); // REV401PLUS cast
    t->coref=result;
    /* Evaluate the copy of the calling term */
    push_goal(eval,copy,result,(GENERIC)t->type->rule); // REV401PLUS cast
    /* Avoid evaluation for built-in functions with unevaluated arguments */
    /* (cond and such_that) */
    wl_node_ptr_ptr::check_func_flag=TRUE;
    if (t->type==iff) {
      get_one_arg(t->attr_list,&t1);
      if (t1) {
        check_out(t1);
      }
    }
    else if(t->type==disjunction) {
    }
    else if (t->type!=such_that) {
      if (t->type->evaluate_args)
        eval_args(t->attr_list);
    }
  }
}
/******** CHECK_TYPE(t)
	  Here we deal with a type which may need checking.
	  This routine will have to be modified to deal with the infinite loops
	  currently caused by definitions such as:

	  :: H:husband(spouse => wife(spouse => H)).
	  :: W:wife(spouse => husband(spouse => W)).

*/
long long check_type(ptr_psi_term t)
// ptr_psi_term t;
{
  long long flag=FALSE;

  push2_ptr_value(int_ptr,(GENERIC *)&(t->status),(GENERIC)(t->status & SMASK)); // REV401PLUS casts
  /* push_ptr_value(int_ptr,&(t->status)); */
  if (t->type->properties) {
    if (t->attr_list || t->type->always_check) {
      /* Check all constraints here: */
      fetch_def(t, TRUE); /* PVR 18.2.94 */
      eval_args(t->attr_list);
      flag=FALSE;
    }
    else {
      /* definition pending on more information */
      t->status= (2 & SMASK) | (t->status & RMASK);
      flag=TRUE;
    }
  }
  else {
    flag=eval_args(t->attr_list);
  }
  return flag;
}
/******** CHECK_OUT(t)
This routine checks out psi_term T.
It deals with the following cases:
- T is a conjunction,
- T is a type which has properties to check.
- The same for T's arguments.
If any of the above holds then proof has to be suspended until the
case has been dealt with.  This is done by pushing goals on the goal_stack
to handle the case.  If all is dealt with then CHECK_OUT returns TRUE.
I.e., CHECK_OUT returns TRUE iff it has not pushed any goals on the stack.

Evaluation is *not* done here, but as a part of dereferencing when a value
is needed.

Of all the routines related to check_out, only i_check_out, check_func,
i_eval_args, and the dereference routines are called from outside of this
file (lefun.c).
- i_check_out(t) checks out everything except functions.  When a function
is encountered, check_out returns immediately without looking inside it.
- f_check_out(t) checks out functions too.
- i_eval_args(n) checks out all arguments, except functions.
- check_func(t) checks out a function & all its arguments (including all
nested functions.  This is done as part of dereferencing, which is part
of unification, matching, built-ins, and user-defined routines.
*/
long long i_check_out(ptr_psi_term t)
// ptr_psi_term t;
{
  wl_node_ptr_ptr::check_func_flag=FALSE;
  return check_out(t);
}
long long f_check_out(ptr_psi_term t)
// ptr_psi_term t;
{
  wl_node_ptr_ptr::check_func_flag=TRUE;
  return check_out(t);
}
long long check_out(ptr_psi_term t)
// ptr_psi_term t;
{
  long long flag=FALSE;
  
  deref_ptr(t);
  if (t->status || (GENERIC)t>=wl_mem->heap_pointer_val()) /*  RM: Feb  8 1993  */
    flag=TRUE;
  else {
    t->status |= RMASK;
    switch((long long)t->type->type_def) { /*  RM: Feb  8 1993  */
    case function_it:
      if (wl_node_ptr_ptr::check_func_flag) {
	check_func(t);
	flag=TRUE;
      }
      else {
	/* Function evaluation handled during matching and unification */
	flag=TRUE;
      }
      break;
    case type_it:
      flag=check_type(t);
      break;
    case global_it: /*  RM: Feb  8 1993  */
      ((wl_psi_term_ptr*)t)->eval_global_var();
      check_out(t);
      flag=FALSE;
      break;
    default:
      flag=eval_args(t->attr_list);
    }
    t->status &= ~RMASK;
  }
  return flag;	
}
void deref_rec_args(ptr_node n)
// ptr_node n;
{
  ptr_psi_term t1;
  
  if (n) {
    deref_rec_args(n->right_val());
    t1 = (ptr_psi_term) (n->data_val());
    deref_ptr(t1);
    //    deref_rec_body(t1);
    ((wl_psi_term_ptr*)t1)->deref_rec_body();
    deref_rec_args(n->left_val());
  }
}
/* Return TRUE iff string (considered as number) is in the set */
/* This routine only recognizes the strings "1", "2", "3",     */
/* represented as numbers 1, 2, 4.                             */
long long in_set(char *str,long long set)
// char *str;
// long long set;
{
  if (set&1 && !featcmp(str,"1")) return TRUE;
  if (set&2 && !featcmp(str,"2")) return TRUE;
  if (set&4 && !featcmp(str,"3")) return TRUE;
  if (set&8 && !featcmp(str,"4")) return TRUE;
  return FALSE;
}
void deref_rec_args_exc(ptr_node n,long long set)
// ptr_node n;
//long long set;
{
  ptr_psi_term t;
  
  if (n) {
    deref_rec_args_exc(n->right_val(),set);
    if (!in_set(n->key_val(),set)) {
      t = (ptr_psi_term) (n->data_val());
      deref_ptr(t);
      //      deref_rec_body(t);
      ((wl_psi_term_ptr*)t)->deref_rec_body();
    }
    deref_rec_args_exc(n->left_val(),set);
  }
}
/********************************************************************/
/* Saving & restoring residuation information */
void save_resid(ptr_resid_block rb,ptr_psi_term match_date)
// ptr_resid_block rb;
// ptr_psi_term match_date;
{
  if (rb) {
    rb->cc_cr = (can_curry<<1) + curried; /* 11.9 */
    rb->ra = resid_aim;
    rb->rv = resid_vars;
    /* rb->cr = curried; 11.9 */
    /* rb->cc = can_curry; 11.9 */
    rb->md = match_date;
  }
}
void restore_resid(ptr_resid_block rb,ptr_psi_term *match_date)
// ptr_resid_block rb;
// ptr_psi_term *match_date;
{
  if (rb) {
    can_curry = (rb->cc_cr&2)?TRUE:FALSE; /* 11.9 */
    curried   = (rb->cc_cr&1)?TRUE:FALSE; /* 11.9 */
    resid_aim = rb->ra;
    resid_vars = rb->rv;
    /* curried = rb->cr; 11.9 */
    /* can_curry = rb->cc; 11.9 */
    *match_date = rb->md;
  }
}
/******** INIT_GLOBAL_VARS()
	  Initialize all non-persistent global variables.
*/
void init_global_vars()  /*  RM: Feb 15 1993  */
{
  ptr_definition def;
  
  for(def=first_definition;def;def=def->next)
    if((GENERIC)(def->global_value)<(GENERIC)wl_mem->heap_pointer_val())
      def->global_value=NULL;
}

/********************************************************************/
