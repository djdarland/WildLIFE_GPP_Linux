#define EXTERN extern
#include "defs.h"
long long wl_psi_term_ptr::curr_status;
long long wl_psi_term_ptr::mark_nonstrict_flag;

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
        if (t->attr_list) ((wl_node_ptr*)t->attr_list)->mark_quote_tree_c(heap_flag);
      }
    }
    else { /* u does not exist yet */ /* 15.9 */
      /* Create a stub & mark it as to-be-quoted. */
      u=NEW(t,psi_term);
      ((wl_psi_term_ptr*)t)->insert_translation(u,QUOTE_STUB);
    }
  }
}
///////////////////////////


/******** COPY(t)
This is the workhorse of the interpreter (alas!).
All copy-related routines are non-interruptible by the garbage collector.
  
Make a copy in the STACK or in the HEAP of psi_term t, which is located in
the HEAP.  A copy is done whenever invoking a rule, so it had better be fast.
This routine uses hash tables with buckets and partial inlining for speed.

The following three versions of copy all rename their variables and return
a completely dereferenced object:

u=exact_copy(t,hf)  u is an exact copy of t.
u=quote_copy(t,hf)  u is a copy of t that is recursively marked evaluated.
u=eval_copy(t,hf)   u is a copy of t that is recursively marked unevaluated.

This version of copy is an incremental copy to the heap.  It copies only
those parts of a psi_term that are on the stack, leaving the others
unchanged:

u=inc_heap_copy(t)  u is an exact copy of t, on the heap.  This is like
hf==HEAP, except that objects already on the heap are
untouched.  Relies on no pointers from heap to stack.

hf = heap_flag.  hf = HEAP or STACK means allocate in the HEAP or STACK.
Marking eval/uneval is done by modifying the STATUS field of the copied
psi_term.
In eval_copy, a term's status is set to 0 if the term or any subterm needs
evaluation.
Terms are dereferenced when copying them to the heap.
*/
#define EXACT_FLAG 0
#define QUOTE_FLAG 1
#define EVAL_FLAG  2
/* See mark_quote_c: */ /* 15.9 */
#define QUOTE_STUB 3
ptr_psi_term wl_psi_term_ptr::exact_copy(long long heap_flag)
// ptr_psi_term t;
// long long heap_flag;
{
  ptr_psi_term t;
  t = (ptr_psi_term) this;
  
  to_heap=FALSE;
  return (((wl_psi_term_ptr*)t)->copy(EXACT_FLAG, heap_flag));
}

ptr_psi_term wl_psi_term_ptr::quote_copy(long long heap_flag)
// ptr_psi_term t;
// long long heap_flag;
{
  ptr_psi_term t;
  t = (ptr_psi_term) this;
  to_heap=FALSE;
  return (((wl_psi_term_ptr*)t)->copy(QUOTE_FLAG, heap_flag));
}

ptr_psi_term wl_psi_term_ptr::eval_copy(long long heap_flag)
// ptr_psi_term t;
// long long heap_flag;
{
  ptr_psi_term t;
  t = (ptr_psi_term) this;
  to_heap=FALSE;
  return (((wl_psi_term_ptr*)t)->copy(EVAL_FLAG, heap_flag));
}

/* There is a bug in inc_heap_copy */
ptr_psi_term wl_psi_term_ptr::inc_heap_copy()
// ptr_psi_term t;
{
  ptr_psi_term t;
  t = (ptr_psi_term) this;
  
  to_heap=TRUE;
  return (((wl_psi_term_ptr*)t)->copy(EXACT_FLAG, TRUE));
}

// static long long curr_status;
ptr_psi_term wl_psi_term_ptr::copy(long long copy_flag, long long heap_flag)
//     ptr_psi_term t;
//     long long copy_flag,heap_flag;
{
  ptr_psi_term u;
  long long old_status;
  long long local_copy_flag;
  long long *infoptr;
  ptr_psi_term t;
  t = (ptr_psi_term) this;
  
  if (u=t) {    
    deref_ptr(t); /* Always dereference when copying */
    if (HEAPDONE(t)) return t;
    u = ((wl_psi_term_ptr*)t)->translate(&infoptr);
    if (u && *infoptr!=QUOTE_STUB) { /* 24.8 */
      /* If it was eval-copied before, then quote it now. */
      if (*infoptr==EVAL_FLAG && copy_flag==QUOTE_FLAG) { /* 24.8 25.8 */
	if (t) ((wl_psi_term_ptr*)t)->mark_quote_c(heap_flag);
	*infoptr=QUOTE_FLAG; /* I.e. don't touch this term any more */
      }
      if (copy_flag==EVAL_FLAG) { /* PVR 14.2.94 */
	/* If any subterm has zero curr_status (i.e., if u->status==0),
	   then so does the whole term: */
	old_status=wl_psi_term_ptr::curr_status;
	wl_psi_term_ptr::curr_status=u->status;
	if (wl_psi_term_ptr::curr_status) wl_psi_term_ptr::curr_status=old_status;
      }
    }
    else {
      if (wl_mem->heap_pointer_val()-wl_mem->stack_pointer_val() < COPY_THRESHOLD) {
	Errorline("psi-term too large -- get a bigger Life!\n");
	abort_life(TRUE);
	longjmp(env,FALSE); /* Back to main loop */ /*  RM: Feb 15 1993  */
      }
      if (copy_flag==EVAL_FLAG && !t->type->evaluate_args) /* 24.8 25.8 */
	local_copy_flag=QUOTE_FLAG; /* All arguments will be quoted 24.8 */
      else /* 24.8 */
	local_copy_flag=copy_flag;
      if (copy_flag==EVAL_FLAG) {
	old_status = curr_status;
	curr_status = 4;
      }
      if (u) { /* 15.9 */
	*infoptr=QUOTE_FLAG;
	local_copy_flag=QUOTE_FLAG;
	copy_flag=QUOTE_FLAG;
      }
      else {
	u=NEW(t,psi_term);
	((wl_psi_term_ptr*)t)->insert_translation(u,local_copy_flag); /* 24.8 */
      }
      *u = *t;
      u->resid=NULL; /* 24.8 Don't copy residuations */
#ifdef TS
      u->time_stamp=global_time_stamp; /* 9.6 */
#endif
      if (t->attr_list)
	u->attr_list=((wl_node_ptr*)t->attr_list)->copy_tree(local_copy_flag, heap_flag);
      if (copy_flag==EVAL_FLAG) {
	switch((long long)t->type->type_def) {
	case type_it:
	  if (t->type->properties)
	    curr_status=0;
	  break;
	case function_it:
	  curr_status=0;
	  break;
	case global_it: /*  RM: Feb  8 1993  */
	  curr_status=0;
	  break;
	default:
	  break;
	}
	u->status=curr_status;
	u->flags=curr_status?QUOTED_TRUE:FALSE; /* 14.9 */
	/* If any subterm has zero curr_status,
	   then so does the whole term: */
	if (curr_status) curr_status=old_status;
      } else if (copy_flag==QUOTE_FLAG) {
	u->status=4;
	u->flags=QUOTED_TRUE; /* 14.9 */
      }
      /* else copy_flag==EXACT_FLAG & u->status=t->status */
      if (heap_flag==HEAP) {
	if (t->type==cut) u->value_3=NULL;
      }	else {
	if (t->type==cut) {
	  u->value_3=(GENERIC)choice_stack;
	  Traceline("current choice point is %x\n",choice_stack);
	}
      }
    }
  }
  return u;
}

/******** DISTINCT_COPY(t)
Make a distinct copy of T and T's attribute tree, which are identical to T,
only located elsewhere in memory. This is used by apply to build the calling
psi-term which is used for matching.  Note that this routine is not
recursive, i.e. it only copies the main functor & the attribute tree.
*/
ptr_psi_term wl_psi_term_ptr::distinct_copy()
// ptr_psi_term t;
{
  ptr_psi_term res;
  ptr_psi_term t;

  t = (ptr_psi_term) this;
  
  res=STACK_ALLOC(psi_term);
  *res= *t;
#ifdef TS
  res->time_stamp=global_time_stamp; /* 9.6 */
#endif
  /* res->coref=distinct_copy(t->coref); */
  if (t->attr_list)
    res->attr_list=((wl_node_ptr*)t->attr_list)->distinct_tree();
  else
    res->attr_list=NULL;
  return res;
}
////////////////////////////////
/* Mark a psi-term as to be evaluated (i.e. strict), except for arguments   */
/* of a nonstrict term, which are marked quoted.  Set status correctly and  */
/* propagate zero status upwards.  Avoid doing superfluous work: non-shared */
/* terms are traversed once; shared terms are traversed at most twice (this */
/* only occurs if the first occurrence encountered is strict and a later    */
/* occurrence is nonstrict).  The translation table is used to indicate (1) */
/* whether a term has already been traversed, and if so, (2) whether there  */
/* was a nonstrict traversal (in that case, the info field is FALSE). */
void wl_psi_term_ptr::mark_eval() /* 24.8 25.8 */
// ptr_psi_term t;
{
  ptr_psi_term t;

  t = (ptr_psi_term) this;
  wl_bucks->clear_copy();
  mark_nonstrict_flag=FALSE;
  ((wl_psi_term_ptr*)t)->mark_eval_new();
}
/* Same as above, except that the status is only changed from 0 to 4 when */
/* needed; it is never changed from 4 to 0. */
void wl_psi_term_ptr::mark_nonstrict()
// ptr_psi_term t;
{
  ptr_psi_term t;

  t = (ptr_psi_term) this;
  wl_bucks->clear_copy();
  mark_nonstrict_flag=TRUE;
  ((wl_psi_term_ptr*)t)->mark_eval_new();
}
/* Mark a term as quoted. */
void wl_psi_term_ptr::mark_quote_new2()
// ptr_psi_term t;
{
  ptr_psi_term t;

  t = (ptr_psi_term) this;
  wl_bucks->clear_copy();
  mark_nonstrict_flag=FALSE;
  ((wl_psi_term_ptr*)t)->mark_quote_new();
}
void wl_psi_term_ptr::mark_eval_new()
// ptr_psi_term t;
{
  ptr_list l;
  long long *infoptr,flag;
  ptr_psi_term u;
  long long old_status;
  ptr_psi_term t;

  t = (ptr_psi_term) this;

  if (t) {
    deref_ptr(t);
    flag = t->type->evaluate_args;
    u=((wl_psi_term_ptr*)t)->translate(&infoptr);
    if (u) {
      /* Quote the subgraph if it was already copied as to be evaluated. */
      if (!flag && *infoptr) {
        ((wl_psi_term_ptr*)t)->mark_quote_new();
        *infoptr=FALSE;
      }
      /* If any subterm has zero curr_status (i.e., if t->status==0),
	 then so does the whole term: PVR 14.2.94 */
      old_status=curr_status;
      curr_status=t->status;
      if (curr_status) curr_status=old_status;
    }
    else {
      ((wl_psi_term_ptr*)t)->insert_translation((ptr_psi_term)TRUE,flag);
      old_status=curr_status;
      curr_status=4;
      if (flag) /* 16.9 */
        {if (t->attr_list) ((wl_node_ptr*)t->attr_list)->mark_eval_tree_new();}
      else
	{if (t->attr_list) ((wl_node_ptr*)t->attr_list)->mark_quote_tree_new();}
      switch((long long)t->type->type_def) {
      case type_it:
        if (t->type->properties)
          curr_status=0;
        break;
      case function_it:
        curr_status=0;
        break;
      case global_it: /*  RM: Feb  8 1993  */
        curr_status=0;
        break;
      default:
	break;
      }
      if (mark_nonstrict_flag) { /* 25.8 */
        if (curr_status) {
          /* Only increase the status, never decrease it: */
          t->status=curr_status;
        }
      }
      else {
        t->status=curr_status;
        t->flags=curr_status?QUOTED_TRUE:FALSE; /* 14.9 */
      }
      /* If any subterm has zero curr_status, then so does the whole term: */
      if (curr_status) curr_status=old_status;
    }
  }
}
///////////////////////////
void wl_psi_term_ptr::mark_quote_new()
// ptr_psi_term t;
{
  ptr_list l;
  long long *infoptr;
  ptr_psi_term u;
  ptr_psi_term t;

  t = (ptr_psi_term) this;

  //  if (t) {
    deref_ptr(t);
    u=((wl_psi_term_ptr*)t)->translate(&infoptr);
    /* Return if the subgraph is already quoted. */
    if (u && !*infoptr) return;
    /* Otherwise quote the subgraph */
    if (!u) ((wl_psi_term_ptr*)t)->insert_translation((ptr_psi_term)TRUE,FALSE);
    else *infoptr = FALSE;	/* sanjay */
    t->status=4;
    t->flags=QUOTED_TRUE; /* 14.9 */
    if (t->attr_list) ((wl_node_ptr*)t->attr_list)->mark_quote_tree_new();
    //  }
}
///////////////////
void wl_psi_term_ptr::mark_quote()
// ptr_psi_term t;
{
  ptr_list l;
  ptr_psi_term t;

  t = (ptr_psi_term) this;

  if (t && !(t->status&RMASK)) {
    t->status = 4;
    t->flags=QUOTED_TRUE; /* 14.9 */
    t->status |= RMASK;
    if (t->coref)
      ((wl_psi_term_ptr*)t->coref)->mark_quote();
    if (t->attr_list)
      ((wl_node_ptr*)t->attr_list)->mark_quote_tree();
    t->status &= ~RMASK;
  }
}
void wl_psi_term_ptr::bk_mark_quote()
// ptr_psi_term t;
{
  ptr_list l;
  ptr_psi_term t;

  t = (ptr_psi_term) this;
  if (t && !(t->status&RMASK)) {
    if(t->status!=4 && (GENERIC)t<wl_mem->heap_pointer_val())/*  RM: Jul 16 1993  */
      push_ptr_value(int_ptr,(GENERIC *)&(t->status)); // REV401PLUS cast
    t->status = 4;
    t->flags=QUOTED_TRUE; /* 14.9 */
    t->status |= RMASK;
    if (t->coref)
      ((wl_psi_term_ptr*)t->coref)->bk_mark_quote();
    if (t->attr_list) ((wl_node_ptr*)t->attr_list)->bk_mark_quote_tree();
    t->status &= ~RMASK;
  }
}
//////////////////////

/* Create an empty list on the stack,  wiped out by RM: Dec 14 1992  */
/* ptr_psi_term stack_empty_list()  is now aliased to stack_nil()    */
/******** RESIDUATE_DOUBLE(t,u)
Residuate the current expression with T in the Residuation Variable set.
Also store the other variable, so that its sort can be used in the
'bestsort' calculation needed to implement disequality constraints.
*/
void wl_psi_term_ptr::residuate_double(ptr_psi_term u) /* 21.9 */
// ptr_psi_term t,u;
{
  ptr_psi_term t;
  ptr_resid_list curr;

  t = (ptr_psi_term) this;
  curr=STACK_ALLOC(resid_list);
  curr->var=t;
  curr->othervar=u;
  curr->next=resid_vars;
  resid_vars=curr;
}
/******** RESIDUATE(t)
Residuate the current expression with T in the Residuation Variable set.
*/
void wl_psi_term_ptr::residuate()
// ptr_psi_term t;
{
  ptr_resid_list curr;
  ptr_psi_term t;

  t = (ptr_psi_term) this;
  curr=STACK_ALLOC(resid_list);
  curr->var=t;
  curr->othervar=NULL; /* 21.9 */
  curr->next=resid_vars;
  resid_vars=curr;
}
/******** RESIDUATE2(u,v)
	  Residuate the current function on the two variables U and V.
*/
void wl_psi_term_ptr::residuate2(ptr_psi_term v)
// /ptr_psi_term u,v;
{
  ptr_psi_term u;
  
  u = (ptr_psi_term) this;
  ((wl_psi_term_ptr*)u)->residuate();
  if (v && u!=v) ((wl_psi_term_ptr*)v)->residuate();
}
/******** RESIDUATE3(u,v,w)
	  Residuate the current function on the three variables U, V, and W.
*/
void wl_psi_term_ptr::residuate3(ptr_psi_term v,ptr_psi_term w)
// ptr_psi_term u,v,w;
{
  ptr_psi_term u;
  
  u = (ptr_psi_term) this;
  ((wl_psi_term_ptr*)u)->residuate();
  if (v && u!=v) ((wl_psi_term_ptr*)v)->residuate();
  if (w && u!=w && v!=w) ((wl_psi_term_ptr*)w)->residuate();
}

//////////////////////

/******** RELEASE_RESID(t)
Release the residuations pending on the Residuation Variable T.
This is done by simply pushing the residuated goals onto the goal-stack.
A goal is not added if already present on the stack.
Two versions of this routine exist: one which trails t and one which never
trails t.
*/
void wl_psi_term_ptr::release_resid_main(long long trailflag)
// ptr_psi_term t;
// long long trailflag;
{
  ptr_goal g;
  ptr_residuation r;
  ptr_psi_term t;
  t = (ptr_psi_term) this;
  
  if (r=t->resid) {
    if (trailflag) push_ptr_value(resid_ptr,(GENERIC *)&(t->resid)); // REV401PLUS cast
    t->resid=NULL;
      while (r) {
      g=r->goal;
      if (g->pending) {
	push_ptr_value(int_ptr,(GENERIC *)&(g->pending)); // REV401PLUS cast
	g->pending=FALSE;
	push_ptr_value(goal_ptr,(GENERIC *)&(g->next)); // REV401PLUS cast
	g->next=goal_stack;
	goal_stack=g;
        Traceline("releasing %P\n",g->aaaa_1);
      }
      r=r->next;
    }
  }
}
void wl_psi_term_ptr::release_resid()
// ptr_psi_term t;
{
  ptr_psi_term t;

  t = (ptr_psi_term) this;
  
  ((wl_psi_term_ptr*)t)->release_resid_main(TRUE);
}
void wl_psi_term_ptr::release_resid_notrail()
// ptr_psi_term t;
{
  ptr_psi_term t;

  t = (ptr_psi_term) this;
  ((wl_psi_term_ptr*)t)->release_resid_main(FALSE);
}
/******** APPEND_RESID(u,v)
Append the residuations pending on V to U. This routine does not check that
the same constraint is not present twice in the end on U. This doesn't matter
since RELEASE_RESID ensures that the same constraint is not released more
than once.
*/
void wl_psi_term_ptr::append_resid(ptr_psi_term v)
// ptr_psi_term u,v;
{
  ptr_residuation *g;

  ptr_psi_term u;
  u = (ptr_psi_term) this;
  
  g= &(u->resid);
  while (*g)
    g = &((*g)->next);
  push_ptr_value(resid_ptr,(GENERIC *)g); // REV401PLUS cast
  *g=v->resid;
}
