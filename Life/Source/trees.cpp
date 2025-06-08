/* Copyright 1991 Digital Equipment Corporation.
** All Rights Reserved.
*****************************************************************/
/* 	$Id: trees.c,v 1.3 1995/07/27 21:22:21 duchier Exp $	 */
#define EXTERN extern
#define REV401PLUS
#ifdef REV401PLUS
#include "defs.h"
#endif
/******** INTCMP(a,b)
	  Compares two integers, for use in FIND or INSERT.
*/
long long intcmp(long long a,long long b)
// long long a;
// long long b;
{
  return a-b;
}
/* Return TRUE iff the string s represents an integer. */
/* Modify s to point to first non-zero digit. */
/* Return number of significant digits in the integer and its sign. */
long long is_int(char **s, long long *len, long long *sgn)
// char **s;
// long long *len;
// long long *sgn;
{
  char *sint; /* Ptr to first non-zero digit */
  char *stmp; /* Scratchpad for string ptr */
  stmp=(*s);
  if (*sgn=(*stmp=='-')) {
    stmp++;
    if (!*stmp) return FALSE;
  }
  if (!*stmp) return FALSE; /* 6.10 */
  while (*stmp=='0') stmp++;
  sint=stmp;
  while (*stmp) {
    if (*stmp<'0' || *stmp>'9') return FALSE;
    stmp++;
  }
  *len=stmp-sint;
  *s=sint;
  return TRUE;
}
/******** FEATCMP(s1,s2)
	  Compares two strings which represent features, for use
	  in FIND or INSERT.  This differs from strcmp for those strings
	  that represent integers.  These are compared as integers.
	  In addition, all integers are considered to be less than
	  all strings that do not represent integers.
*/
long long featcmp(char *str1, char *str2)
// char *str1, *str2;
{
  long long len1,len2,sgn1,sgn2;
  char *s1,*s2;

  if(str1==str2)
    return 0;
  if(*(str1+1)==0 && *(str2+1)==0)
    return *str1 - *str2;
  s1=str1; /* Local copies of the pointers */
  s2=str2;
  if (is_int(&s1,&len1,&sgn1)) {
    if (is_int(&s2,&len2,&sgn2)) {
      if (sgn1!=sgn2) return (sgn2-sgn1); /* Check signs first */
      if (len1!=len2) return (len1-len2); /* Then check lengths */
      return strcmp(s1,s2); /* Use strcmp only if same sign and length */
    }
    else
      return -1;
  }
  else {
    if (is_int(&s2,&len2,&sgn2))
      return 1;
    else
      return strcmp(s1,s2);
  }
}
/******** HEAP_NCOPY_STRING(string,length)
	  Make a copy of the string in the heap, and return a pointer to that.
	  Exceptions: "1" and "2" are unique (and in the heap).
*/
char *heap_ncopy_string(char *s,int n)
// char *s;
// int n;
{
  char *p;
  
  if (s==one || s==two) return s;
  p=(char *)wl_mem->heap_alloc(n+1);
  strncpy(p,s,n);
  p[n]='\0';
  return p;
}
/******** HEAP_COPY_STRING(string)
	  Make a copy of the string in the heap, and return a pointer to that.
	  Exceptions: "1" and "2" are unique (and in the heap).
*/
char *heap_copy_string(char *s)
// char *s;
{ return heap_ncopy_string(s,strlen(s)); }
/******** STACK_COPY_STRING(string)
	  Make a copy of the string in the stack, and return a pointer to that.
	  Exceptions: "1" and "2" are unique (and in the heap).
*/
char *stack_copy_string(char *s)
// char *s;
{
  char *p;
  if (s==one || s==two) return s;
  p=(char *)wl_mem->stack_alloc(strlen(s)+1);
  strcpy(p,s);
  return p;
}
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
ptr_node general_insert(long long comp,char *keystr,
			ptr_node *tree,GENERIC info,
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
void heap_insert_copystr(char *keystr,ptr_node *tree, GENERIC info)
// char *keystr;
// ptr_node *tree;
// GENERIC info;
{
  general_insert(FEATCMP,keystr,tree,info,HEAP,TRUE,0);
}
/******** STACK_INSERT_COPYSTR(keystr,tree,info)
Insert the pointer INFO under the reference string KEYSTR (which is
a feature name) in the binary tree TREE.  KEYSTR is copied to the heap.
A potential additional node allocated to TREE is put on the stack.
*/
void stack_insert_copystr(char *keystr,ptr_node *tree,GENERIC info)
// char *keystr;
// ptr_node *tree;
// GENERIC info;
{
  general_insert(FEATCMP,keystr,tree,info,STACK,TRUE,0);
}
/******** HEAP_INSERT(comp,keystr,tree,info)
Insert the pointer INFO under the reference KEYSTR in the
binary tree TREE stored in the heap.
Return the pointer to the node of KEYSTR.
*/
ptr_node heap_insert(long long comp,char *keystr,ptr_node *tree,GENERIC info)
// long long comp;
// char *keystr;
// ptr_node *tree;
// GENERIC info;
{
  return general_insert(comp,keystr,tree,info,HEAP,FALSE,0);
}
/******** STACK_INSERT(comp,keystr,tree,info)
Exactly the same as heap_insert, only the new node is in the stack.
*/
ptr_node stack_insert(long long comp,char *keystr,ptr_node *tree,GENERIC info)
// long long comp;
// char *keystr;
// ptr_node *tree;
// GENERIC info;
{
  return general_insert(comp,keystr,tree,info,STACK,FALSE,0);
}
/******** BK_STACK_INSERT(comp,keystr,tree,info)
	  Insert the pointer INFO under the reference string KEYSTR of
	  length len in the binary tree TREE. Return the pointer to the permanent
	  storage place of KEY. This is used by C_APPLY_LABEL
	  Trail the change with a trail check.
*/
ptr_node bk_stack_insert(long long comp,char *keystr,ptr_node *tree,GENERIC info)
// long long comp;
// char *keystr;
// ptr_node *tree;
// GENERIC info;
{
  return general_insert(comp,keystr,tree,info,STACK,FALSE,1);
}
/******** BK2_STACK_INSERT(comp,keystr,tree,info)
Insert the pointer INFO under the reference string KEYSTR of
length len in the binary tree TREE. Return the pointer to the permanent
storage place of KEY. This is used by C_APPLY_LABEL
Always trail the change.
*/
ptr_node bk2_stack_insert(long long comp,char *keystr,ptr_node *tree,GENERIC info)
// long long comp;
// char *keystr;
// ptr_node *tree;
// GENERIC info;
{
  return general_insert(comp,keystr,tree,info,STACK,FALSE,2);
}
/******** FIND(comp,keystr,tree)
Return the NODE address corresponding to key KEYSTR in TREE using function
COMP to compare keys.
*/
ptr_node find(long long comp,char *keystr,ptr_node tree)
// long long comp;
//char *keystr;
// ptr_node tree;
{
  ptr_node result;
  long long cmp;
  long long to_do=TRUE;
    
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
ptr_node find_data(GENERIC p,ptr_node t)
// GENERIC p;
// ptr_node t;
{
  ptr_node r=NULL;

  if(t) 
    if(t->data==p)
      r=t;
    else {
      r=find_data(p,t->left);
      if(r==NULL)
	r=find_data(p,t->right);
    }
  return r;
}
/******** DELETE_ATTR(key,tree)
	  Remove the node addressed by KEY from TREE.
*/
void delete_attr(char *s,ptr_node *n)
// char *s;
// ptr_node *n;
{
  long long cmp;
  ptr_node wl_new,r;

  if (*n) {
    cmp=featcmp(s,(*n)->key);
    if (cmp<0)
      delete_attr(s,&((*n)->left));
    else if (cmp>0)
      delete_attr(s,&((*n)->right));
    else if ((*n)->left) {
      if ((*n)->right) {
        r=(*n)->right;
        wl_new=heap_insert(FEATCMP,r->key,&((*n)->left),r->data);
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
