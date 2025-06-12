#define EXTERN extern
#include "defs.h"
/******** GENERAL_INSERT(comp,keystr,tree,info,heapflag,copystr,bkflag)
General tree insertion routine.
comp     = comparison routine for insertion.
keystr   = the insertion key.
tree     = the tree to insert in.
info     = the information to insert.
heapflag = HEAP or STACK for heap or stack allocation of insertion node.
copystr  = TRUE iff copy the keystr to the heap on insertion.
bkflag   = 1 iff the insertion is backtrackable (trailed with trail check).
2 iff the insertion must always be trailed.
Returns a pointer to the node containing the pair (keystr,info).

Here KEYSTR can be either a pointer to a string, an integer, or a feature.
COMP is the function to call to compare 2 keys so it has three
possible values: COMP==strcmp(), COMP==intcmp(), or COMP==featcmp().
COMP(a,b) should return n where: n=0 if a=b; n>0 if a>b; n<0 if a<b.
*/
ptr_node wl_node_ptr_ptr::general_insert(long long comp,char *keystr,
			GENERIC info,
			long long heapflag, long long copystr,
			long long bkflag)
// long long comp;
// char *keystr;
// ptr_node *tree;
// GENERIC info;
// long long heapflag, copystr, bkflag;
{
  long long cmp;
  ptr_node result;
  long long to_do=TRUE;
  ptr_node * tree;
  
  tree = (ptr_node *) this;
  do {
    if (*tree==NULL) {
      if (bkflag==1) push_ptr_value(int_ptr,(GENERIC *)tree); // REV401PLUS cast
      else if (bkflag==2) push_ptr_value_global(int_ptr,(GENERIC *)tree); // REV401PLUS cast
      *tree = (heapflag==HEAP) ? HEAP_ALLOC(node): STACK_ALLOC(node);
      result= *tree;
      (*tree)->key = copystr ? heap_copy_string(keystr) : keystr;
      (*tree)->left=NULL;
      (*tree)->right=NULL;
      (*tree)->data=info;
      to_do=FALSE;
    }
    else {
      // revised logic not using function pointer
      if (comp == INTCMP) cmp=intcmp((long long)keystr,(long long)(*tree)->key); // REV401PLUS cast
      else if (comp == STRCMP) cmp=strcmp(keystr,(*tree)->key);
      else if (comp == FEATCMP) cmp=featcmp(keystr,(*tree)->key);
      if (cmp<0)
	tree=(&((*tree)->left));
      else
	if (cmp==0) {
          if (bkflag)
            Errorline("attempt to overwrite an existing feature; ignored.\n");
          else
	    (*tree)->data=info;
	  result= *tree;
	  to_do=FALSE;
	}
	else
	  tree=(&((*tree)->right));
    }
  } while (to_do);
  return result;
}
/******** HEAP_INSERT_COPYSTR(keystr,tree,info)
Insert the pointer INFO under the reference string KEYSTR (which is
a feature name) in the binary tree TREE.  KEYSTR is copied to the heap.
A potential additional node allocated to TREE is put on the heap.
*/
void wl_node_ptr_ptr::heap_insert_copystr(char *keystr, GENERIC info)
// char *keystr;
// ptr_node *tree;
// GENERIC info;
{
  ptr_node * tree;
  tree = (ptr_node *) this;

  ((wl_node_ptr_ptr*)tree)->general_insert(FEATCMP,keystr,info,HEAP,TRUE,0);
}
/******** STACK_INSERT_COPYSTR(keystr,tree,info)
Insert the pointer INFO under the reference string KEYSTR (which is
a feature name) in the binary tree TREE.  KEYSTR is copied to the heap.
A potential additional node allocated to TREE is put on the stack.
*/
void wl_node_ptr_ptr::stack_insert_copystr(char *keystr,GENERIC info)
// char *keystr;
// ptr_node *tree;
// GENERIC info;
{
  ptr_node * tree;
  tree = (ptr_node *) this;
  ((wl_node_ptr_ptr*)tree)->general_insert(FEATCMP,keystr,info,STACK,TRUE,0);
}
/******** HEAP_INSERT(comp,keystr,tree,info)
Insert the pointer INFO under the reference KEYSTR in the
binary tree TREE stored in the heap.
Return the pointer to the node of KEYSTR.
*/
ptr_node wl_node_ptr_ptr::heap_insert(long long comp,char *keystr,GENERIC info)
// long long comp;
// char *keystr;
// ptr_node *tree;
// GENERIC info;
{
  ptr_node * tree;
  tree = (ptr_node *) this;
  return ((wl_node_ptr_ptr*)tree)->general_insert(comp,keystr,info,HEAP,FALSE,0);
}
/******** STACK_INSERT(comp,keystr,tree,info)
Exactly the same as heap_insert, only the new node is in the stack.
*/
ptr_node wl_node_ptr_ptr::stack_insert(long long comp,char *keystr,GENERIC info)
// long long comp;
// char *keystr;
// ptr_node *tree;
// GENERIC info;
{
  ptr_node * tree;
  tree = (ptr_node *) this;
  return ((wl_node_ptr_ptr*)tree)->general_insert(comp,keystr,info,STACK,FALSE,0);
}
/******** BK_STACK_INSERT(comp,keystr,tree,info)
	  Insert the pointer INFO under the reference string KEYSTR of
	  length len in the binary tree TREE. Return the pointer to the permanent
	  storage place of KEY. This is used by C_APPLY_LABEL
	  Trail the change with a trail check.
*/
ptr_node wl_node_ptr_ptr::bk_stack_insert(long long comp,char *keystr,GENERIC info)
// long long comp;
// char *keystr;
// ptr_node *tree;
// GENERIC info;
{
  ptr_node * tree;
  tree = (ptr_node *) this;
  return ((wl_node_ptr_ptr*)tree)->general_insert(comp,keystr,info,STACK,FALSE,1);
}
/******** BK2_STACK_INSERT(comp,keystr,tree,info)
Insert the pointer INFO under the reference string KEYSTR of
length len in the binary tree TREE. Return the pointer to the permanent
storage place of KEY. This is used by C_APPLY_LABEL
Always trail the change.
*/
ptr_node wl_node_ptr_ptr::bk2_stack_insert(long long comp,char *keystr,GENERIC info)
// long long comp;
// char *keystr;
// ptr_node *tree;
// GENERIC info;
{
  ptr_node * tree;
  tree = (ptr_node *) this;
  return ((wl_node_ptr_ptr*)tree)->general_insert(comp,keystr,info,STACK,FALSE,2);
}
/******** DELETE_ATTR(key,tree)
	  Remove the node addressed by KEY from TREE.
*/
void wl_node_ptr_ptr::delete_attr(char *s)
// char *s;
// ptr_node *n;
{
  ptr_node *n;
  long long cmp;
  ptr_node wl_new,r;
  n = (ptr_node *) this;

  if (*n) {
    cmp=featcmp(s,(*n)->key);
    if (cmp<0)
      ((wl_node_ptr_ptr*)&((*n)->left))->delete_attr(s);
    else if (cmp>0)
      ((wl_node_ptr_ptr*)&((*n)->right))->delete_attr(s);
    else if ((*n)->left) {
      if ((*n)->right) {
        r=(*n)->right;
        wl_new=((wl_node_ptr_ptr*)&(*n)->left)->heap_insert(FEATCMP,r->key,r->data);
        wl_new->left=r->left;
        wl_new->right=r->right;
        *n = (*n) -> left;
      }
      else
        *n = (*n)->left;
    }
    else
      *n = (*n)->right;
  }
}
