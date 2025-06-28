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

ptr_goal wl_psi_term_ptr::makeGoal()
// ptr_psi_term p;
{
  ptr_psi_term p;
  ptr_goal old = goal_stack;
  ptr_goal g;
  p = (ptr_psi_term) this;
  
  push_goal(prove, p, (ptr_psi_term)DEFRULES, NULL);
  g = goal_stack;
  g->next=NULL;
  goal_stack = old;
  return g;
}
/* Boolean utility function that implements isa */
long long wl_psi_term_ptr::isa(ptr_psi_term arg2)
// ptr_psi_term arg1, arg2;
{
  long long ans;
  ptr_psi_term arg1;

  arg1 = (ptr_psi_term) this;
  
  if (  arg1->type==arg2->type
	|| (  (arg1->type==real || arg1->type==integer)
	      && (arg2->type==real || arg2->type==integer)
	      && (arg1->value_3 || arg2->value_3)
	      )
	) {
    if(arg1->type==cut) /*  RM: Jan 21 1993  */
      ans=TRUE;
    else
      ans=isSubTypeValue(arg1, arg2);
  }
  else {
    matches(arg1->type, arg2->type, &ans);
  }
  return ans;
}
#define isa_le_sel 0
#define isa_lt_sel 1
#define isa_ge_sel 2
#define isa_gt_sel 3
#define isa_eq_sel 4
#define isa_nle_sel 5
#define isa_nlt_sel 6
#define isa_nge_sel 7
#define isa_ngt_sel 8
#define isa_neq_sel 9
#define isa_cmp_sel 10
#define isa_ncmp_sel 11
/* Utility that selects one of several isa functions */
long long wl_psi_term_ptr::isa_select(ptr_psi_term arg2, long long sel)
// ptr_psi_term arg1,arg2;
// long long sel;
{
  long long ans;
  ptr_psi_term arg1;

  arg1 = (ptr_psi_term) this;

  switch (sel) {
  case isa_le_sel: ans = ((wl_psi_term_ptr*)arg1)->isa(arg2);
    break;
  case isa_lt_sel: ans = ((wl_psi_term_ptr*)arg1)->isa(arg2) &&
      !((wl_psi_term_ptr*)arg2)->isa(arg1);
    break;
  case isa_ge_sel: ans = ((wl_psi_term_ptr*)arg2)->isa(arg1);
    break;
  case isa_gt_sel: ans = ((wl_psi_term_ptr*)arg2)->isa(arg1) &&
      !((wl_psi_term_ptr*)arg1)->isa(arg2);
    break;
  case isa_eq_sel: ans=((wl_psi_term_ptr*)arg1)->isa(arg2) &&
      ((wl_psi_term_ptr*)arg2)->isa(arg1);
    break;
  case isa_nle_sel: ans= !((wl_psi_term_ptr*)arg1)->isa(arg2);
    break;
  case isa_nlt_sel: ans= !(((wl_psi_term_ptr*)arg1)->isa(arg2) &&
			   !((wl_psi_term_ptr*)arg2)->isa(arg1));
    break;
  case isa_nge_sel: ans= !((wl_psi_term_ptr*)arg2)->isa(arg1);
    break;
  case isa_ngt_sel: ans= !(((wl_psi_term_ptr*)arg2)->isa(arg1) &&
			   !((wl_psi_term_ptr*)arg1)->isa(arg2));
    break;
  case isa_neq_sel: ans= !(((wl_psi_term_ptr*)arg1)->isa(arg2) &&
			   ((wl_psi_term_ptr*)arg2)->isa(arg1));
    break;
  case isa_cmp_sel: ans=((wl_psi_term_ptr*)arg1)->isa(arg1) ||
      ((wl_psi_term_ptr*)arg2)->isa(arg1);
    break;
  case isa_ncmp_sel: ans= !(((wl_psi_term_ptr*)arg1)->isa(arg2) ||
			    ((wl_psi_term_ptr*)arg2)->isa(arg1));
    break;
  }
  return ans;
}
long long wl_psi_term_ptr::isValue()  // REV401PLUS to long long
// ptr_psi_term p;
{
  ptr_psi_term p;
  p = (ptr_psi_term) this;
  return (p->value_3 != NULL);
}
ptr_psi_term wl_psi_term_ptr::stack_cons(ptr_psi_term tail)
//     ptr_psi_term head;
//     ptr_psi_term tail;
{
  ptr_psi_term cons;
  ptr_psi_term head;
  head = (ptr_psi_term)this;

  cons=stack_psi_term(4);
  cons->type=alist;
  //  if(head)
  ((wl_node_ptr_ptr*)&(cons->attr_list))->stack_insert(FEATCMP,one,(GENERIC)head); //cast REV401PLUS
  if(tail)
    ((wl_node_ptr_ptr*)&(cons->attr_list))->stack_insert(FEATCMP,two,(GENERIC)tail); // cast REV401PLUS
  return cons;
}
/********* STACK_PAIR(left,right)
	   create a PAIR object.
*/
ptr_psi_term wl_psi_term_ptr::stack_pair(ptr_psi_term right)
//     ptr_psi_term left;
//     ptr_psi_term right;
{
  ptr_psi_term pair;
  ptr_psi_term left;
  
  left = (ptr_psi_term) this;
  
  pair=stack_psi_term(4);
  pair->type=wl_and;
  //  if(left)
  ((wl_node_ptr_ptr*)&(pair->attr_list))->stack_insert(FEATCMP,one,(GENERIC)left);  // cast REV401PLUS
  if(right)
    ((wl_node_ptr_ptr*)&(pair->attr_list))->stack_insert(FEATCMP,two,(GENERIC)right);  // cast REV401PLUS
  return pair;
}
/******** INSERT_TRANSLATION(a,b,info)
	  Add the translation of address A to address B in the translation table.
	  Also add an info field.
*/
/* static */ void wl_psi_term_ptr::insert_translation(ptr_psi_term b,long long info)
	     // ptr_psi_term a;
	     // ptr_psi_term b;
	     // long long info;
{
  long long index;
  long long lastbucket;
  ptr_psi_term a;

  a = (ptr_psi_term) this;
  
  /* Ensure there are free buckets by doubling their number if necessary */
  if (wl_bucks->hashfree >= wl_bucks->numbuckets) {
    wl_bucks->numbuckets *= 2;
    wl_bucks->hashbuckets = (struct hashbucket *) 
      realloc((void *) wl_bucks->hashbuckets, wl_bucks->numbuckets * sizeof(struct hashbucket));
    /* *** Do error handling here *** */
    Traceline("doubled the number of hashbuckets to %d\n", wl_bucks->numbuckets);
  }
  /* Add a bucket to the beginning of the list */
  index = HASH(a);
  if (wl_bucks->hashtable[index].timestamp == wl_bucks->hashtime)
    lastbucket = wl_bucks->hashtable[index].bucketindex;
  else {
    lastbucket = HASHEND;
    wl_bucks->hashtable[index].timestamp = wl_bucks->hashtime;
  }
  wl_bucks->hashtable[index].bucketindex = wl_bucks->hashfree;
  wl_bucks->hashbuckets[wl_bucks->hashfree].old_value = a;
  wl_bucks->hashbuckets[wl_bucks->hashfree].new_value = b;
  wl_bucks->hashbuckets[wl_bucks->hashfree].info = info;
  wl_bucks->hashbuckets[wl_bucks->hashfree].next = lastbucket;
  wl_bucks->hashfree++;
}

/******** TRANSLATE(a,info)
	  Get the translation of address A and the info field stored with it.
	  Return NULL if none is found.
*/
/* static */ ptr_psi_term wl_psi_term_ptr::translate(long long **infoptr)   /*  RM: Jan 27 1993  */
	     //  ptr_psi_term a;
	     //long long **infoptr;
{
  long long index;
  /* long long i; 20.8 */
  long long bucket;
  ptr_psi_term a;
  a = (ptr_psi_term) this;

  index = HASH(a);
  if (wl_bucks->hashtable[index].timestamp != wl_bucks->hashtime) return NULL;
  bucket = wl_bucks->hashtable[index].bucketindex;
  /* i=0; 20.8 */
  while (bucket != HASHEND && wl_bucks->hashbuckets[bucket].old_value != a) {
    /* i++; 20.8 */
    bucket = wl_bucks->hashbuckets[bucket].next;
  }
  /* hashstats[i]++; 20.8 */
  if (bucket != HASHEND) {
    *infoptr = &wl_bucks->hashbuckets[bucket].info;
    return (wl_bucks->hashbuckets[bucket].new_value);
  }
  else
    return NULL;
}

/****************************************************************************/
/* Meaning of the info field in the translation table: */
/* With u=translate(t,&infoptr): */
/* If infoptr==QUOTE_FLAG then the whole subgraph from u is quoted. */
/* If infoptr==EVAL_FLAG then anything is possible. */
/* If infoptr==QUOTE_STUB then the term does not exist yet, e.g., there  */
/* is a cycle in the term & copy(...) has not created it yet, for  */
/* example X:s(L,t(X),R), where non_strict(t), in which R does not */
/* exist when the call to mark_quote_c is done.  When the term is  */
/* later created, it must be created as quoted. */

/* Mark a psi-term u (which is a copy of t) as completely evaluated. */
/* Only t is given as the argument. */
/* Assumes the psi-term is a copy of another psi-term t, which is made through */
/* eval_copy.  Therefore the copy is accessible through the translation table. */
/* Assumes all translation table entries already exist. */
/* The infoptr field is updated so that each subgraph is only traversed once. */
/* This routine is called only from the main copy routine. */
void wl_psi_term_ptr::mark_quote_c(long long heap_flag)
// ptr_psi_term t;
// long long heap_flag;
{
  ptr_list l;
  long long *infoptr;
  ptr_psi_term u;
  ptr_psi_term t;
  t = (ptr_psi_term) this;

  if (t) {
    deref_ptr(t);
    u=((wl_psi_term_ptr*)t)->translate(&infoptr);
    /* assert(u!=NULL); 15.9 */
    if (u) {
      if (*infoptr==EVAL_FLAG) {
        *infoptr=QUOTE_FLAG;
        u->status=4;
        u->flags=QUOTED_TRUE; /* 14.9 */
        mark_quote_tree_c(t->attr_list,heap_flag);
      }
    }
    else { /* u does not exist yet */ /* 15.9 */
      /* Create a stub & mark it as to-be-quoted. */
      u=NEW(t,psi_term);
      ((wl_psi_term_ptr*)t)->insert_translation(u,QUOTE_STUB);
    }
  }
}
