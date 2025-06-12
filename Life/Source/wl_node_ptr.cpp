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

