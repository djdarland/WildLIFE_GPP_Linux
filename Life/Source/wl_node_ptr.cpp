#define EXTERN extern
#include "defs.h"

/******** FIND(comp,keystr,tree)
Return the NODE address corresponding to key KEYSTR in TREE using function
COMP to compare keys.
*/
ptr_node wl_node_ptr::find(long long comp,char *keystr)
// long long comp;
//char *keystr;
// ptr_node tree;
{
  ptr_node tree;
  ptr_node result;
  long long cmp;
  long long to_do=TRUE;

  tree = (ptr_node)this;
  do {
    if (tree==NULL) {
      result=NULL;
      to_do=FALSE;
    }
    else {
      // revised logic not using function pointer
      if (comp == INTCMP) cmp=intcmp((long long)keystr,(long long)((struct wl_node *)tree)->key);
      else if (comp == STRCMP) cmp=strcmp((char *)keystr,(char *)((struct wl_node *)tree)->key);
      else if (comp == FEATCMP) cmp=featcmp(keystr,((struct wl_node *)tree)->key);
      if (cmp<0)
	tree=tree->left;
      else
	if (cmp==0) {
	  result=tree;
	  to_do=FALSE;
	}
	else
	  tree=tree->right;
    }
  } while (to_do);
  return result;
}
/******** FIND_DATA(p,t)
Return the node containing the data P in tree T. This is a linear search and
can be used to find the key to some data if it is unkown.
Return NULL if no key corresponds to data P.
*/
ptr_node wl_node_ptr::find_data(GENERIC p)
// GENERIC p;
// ptr_node t;
{
  ptr_node t;

  ptr_node r=NULL;
  t = (ptr_node)this;
  if(t) 
    if(t->data==p)
      r=t;
    else {
      r=((wl_node_ptr*)(t->left))->find_data(p);
      if(r==NULL)
	r=((wl_node_ptr*)(t->right))->find_data(p);
    }
  return r;
}

ptr_psi_term wl_node_ptr::make_feature_list(ptr_psi_term tail,
			       ptr_module module,int val)
//     ptr_node tree;
//     ptr_psi_term tail;
//     ptr_module module;
//     int val;
     
{
  ptr_psi_term wl_new;
  ptr_definition def;
  double d; // , strtod();
  ptr_node tree;

  tree = (ptr_node) this;
  
  if(tree) {
    if(tree->right)
      tail=((wl_node_ptr*)(tree->right))->make_feature_list(tail,module,val);
    /* Insert the feature name into the list */
    d=str_to_int(tree->key);
    if (d== -1) { /* Feature is not a number */
      def=((wl_module_ptr*)module)->update_feature(tree->key); /* Extract module RM: Feb 3 1993 */
      if(def) {
	if(val) /* RM: Mar  3 1994 Distinguish between features & values */
	  tail=((wl_psi_term_ptr*)(ptr_psi_term)tree->data)->stack_cons(tail); // REV401PLUS cast
	else {
	  wl_new=stack_psi_term(4);      
	  wl_new->type=def;
	  tail = ((wl_psi_term_ptr*)wl_new)->stack_cons(tail);
	}
      }
    }
    else { /* Feature is a number */
      if(val) /* RM: Mar  3 1994 Distinguish between features & values */
	tail= ((wl_psi_term_ptr*)((ptr_psi_term)tree->data))->stack_cons(tail); // REV401PLUS cast
      else {
	wl_new=stack_psi_term(4);      
	wl_new->type=(d==floor(d))?integer:real;
	wl_new->value_3=wl_mem->heap_alloc(sizeof(REAL));
	*(REAL *)wl_new->value_3=(REAL)d;
	tail=((wl_psi_term_ptr*)wl_new)->stack_cons(tail);
      }
    }
    if(tree->left)
      tail=((wl_node_ptr*)tree->left)->make_feature_list(tail,module,val);
  }
  return tail;
}

void wl_node_ptr::mark_quote_tree_c(long long heap_flag)
// ptr_node n;
// long long heap_flag;
{
  ptr_node n;

  n = (ptr_node) this;
  if (n) {
    if (n->left) ((wl_node_ptr*)n->left)->mark_quote_tree_c(heap_flag);
    if (n->data) ((wl_psi_term_ptr*)(n->data))->mark_quote_c(heap_flag);
    if (n->right) ((wl_node_ptr*)n->right)->mark_quote_tree_c(heap_flag);
  }
}


ptr_node wl_node_ptr::copy_tree(long long copy_flag, long long heap_flag)
//ptr_node t;
//long long copy_flag, heap_flag;
{
  ptr_node r;
  ptr_psi_term t1,t2;
  ptr_node t;

  t = (ptr_node) this;
  
  if (HEAPDONE(t)) return t;
  r=NEW(t,node);
  r->key = t->key;
  r->left  = (t->left)  ? ((wl_node_ptr*)t->left)->copy_tree(copy_flag,heap_flag)  : NULL;
  t1 = (ptr_psi_term)(t->data);
  t2 = ((wl_psi_term_ptr*)t1)->copy(copy_flag,heap_flag);
  r->data = (GENERIC) t2;
  r->right = (t->right) ? ((wl_node_ptr*)t->right)->copy_tree(copy_flag,heap_flag) : NULL;
  return r;
}

/****************************************************************************/
/******** DISTINCT_TREE(t)
Return an exact copy of an attribute tree.
This is used by APPLY in order to build the calling psi-term which is used
for matching.
*/

ptr_node wl_node_ptr::distinct_tree()
// ptr_node t;
{
  ptr_node n;
  ptr_node t;

  t = (ptr_node) this;
  
  //  n=NULL;
  //  if (t) {
    n=STACK_ALLOC(node);
    n->key=t->key;
    n->data=t->data;
    if (t->left)
    n->left=((wl_node_ptr*)t->left)->distinct_tree();
    else n->left = NULL;
    if (t->right)
    n->right=((wl_node_ptr*)t->right)->distinct_tree();
    else n->right = NULL;
    //  }
  return n;
}

void wl_node_ptr::mark_eval_tree_new()
// ptr_node n;
{
  ptr_node n;

  n = (ptr_node) this;
  //  if (n) {
  if (n->left)
    ((wl_node_ptr*)n->left)->mark_eval_tree_new();
  if (n->data) ((wl_psi_term_ptr*)n->data)->mark_eval_new();
  if (n->right)
    ((wl_node_ptr*)n->right)->mark_eval_tree_new();
  //  }
}

void wl_node_ptr::mark_quote_tree_new()
// ptr_node n;
{
  ptr_node n;

  n = (ptr_node) this;
  //  if (n) {
    if (n->left)
      ((wl_node_ptr*)n->left)->mark_quote_tree_new();
    if (n->data) ((wl_psi_term_ptr*)n->data)->mark_quote_new();
    if (n->right)
      ((wl_node_ptr*)n->right)->mark_quote_tree_new();
    // }
}

void wl_node_ptr::mark_quote_tree()
// ptr_node t;
{
  ptr_node t;

  t = (ptr_node) this;
  //  if (t) {
  if (t->left)
    ((wl_node_ptr*)t->left)->mark_quote_tree();
  if(t->data) ((wl_psi_term_ptr*)t->data)->mark_quote();
  if (t->right)
    ((wl_node_ptr*)t->right)->mark_quote_tree();
  // }
}
void wl_node_ptr::bk_mark_quote_tree()
// ptr_node t;
{
  ptr_node t;

  t = (ptr_node) this;
  //  if (t) {
  if (t->left) ((wl_node_ptr*)t->left)->bk_mark_quote_tree();
  if (t->data) ((wl_psi_term_ptr*)t->data)->bk_mark_quote();
  if (t->right) ((wl_node_ptr*)t->right)->bk_mark_quote_tree();
  //}
}

