#define EXTERN extern
#include "defs.h"
/******** HASH_EXPAND(table,new_size)
	  Allocate a bigger hash table.
*/
void wl_hash_table_ptr::hash_expand(int new_size)
//     ptr_hash_table table;
//     int new_size;
{
  ptr_hash_table table;
  table = (ptr_hash_table) this;
  
  ptr_keyword *old_data;
  int old_size;
  int i;
  
  old_data=table->data;
  old_size=table->size;
  table->size=new_size; /* Must be power of 2 */
  table->used=0;
  table->data=(ptr_keyword *)malloc(new_size*sizeof(ptr_keyword));
  for(i=0;i<new_size;i++)
    table->data[i]=NULL;
  for(i=0;i<old_size;i++)
    if(old_data[i])
      ((wl_hash_table_ptr*)table)->hash_insert(old_data[i]->symbol,old_data[i]);
  free(old_data);
}
/******** HASH_CODE(table,symbol)
	  Return the hash code for a symbol
*/
int wl_hash_table_ptr::hash_code(char *symbol)
//     ptr_hash_table table;
//     char *symbol;
{
  ptr_hash_table table;
  int n=0;

  table = (ptr_hash_table) this;
  
  while(*symbol) {
    n ^= rand_array[*symbol]+rand_array[n&255];
    n++;
    symbol++;
  }
  n &= (table->size-1);
  return n;
}
int wl_hash_table_ptr::hash_find(char *symbol)
//     ptr_hash_table table;
//     char *symbol;
{
  ptr_hash_table table;
  int n;
  int i=1;

  table = (ptr_hash_table) this;
  n=((wl_hash_table_ptr*)table)->hash_code(symbol);
  while(table->data[n] && strcmp(table->data[n]->symbol,symbol)) {
    /* Not a direct hit... */
    n+= i*i;
    /* i++; */
    n &= table->size-1;
  }
  return n;
}
/******** HASH_LOOKUP(table,symbol)
	  Look up a symbol in the symbol table.
*/
ptr_keyword wl_hash_table_ptr::hash_lookup(char *symbol)
//     ptr_hash_table table;
//     char *symbol;
{
  ptr_hash_table table;
  int n;

  table = (ptr_hash_table) this;
  n=((wl_hash_table_ptr*)table)->hash_find(symbol);
  return table->data[n];
}
/******** HASH_INSERT(table,symbol,keyword)
	  Add a symbol and data to a table. Overwrite previous data.
*/
void wl_hash_table_ptr::hash_insert(char *symbol,ptr_keyword keyword)
//     ptr_hash_table table;
//     char *symbol;
//     ptr_keyword keyword;
{
  int n;
  ptr_hash_table table;

  table = (ptr_hash_table) this;
  n=((wl_hash_table_ptr*)table)->hash_find(symbol);
  if(!table->data[n])
    table->used++;
  table->data[n]=keyword;
  if(table->used*2>table->size)
    ((wl_hash_table_ptr*)table)->hash_expand(table->size*2);
}
/******** HASH_DISPLAY(table)
	  Display a symbol table (for debugging).
*/
void wl_hash_table_ptr::hash_display()
//     ptr_hash_table table;
{
  ptr_hash_table table;
  int i;
  int n;
  char *s;
  int c=0;
  int t=0;

  table = (ptr_hash_table) this;
  printf("*** Hash table %llx:\n",(unsigned long long)table); // added ->%lx & cast
  printf("Size: %d\n",table->size);
  printf("Used: %d\n",table->used);
  for(i=0;i<table->size;i++)
    if(table->data[i]) {
      t++;
      s=table->data[i]->symbol;
      n=((wl_hash_table_ptr*)table)->hash_code(s);
      printf("%4d %4d %s %s\n",
	     i,
	     n,
	     i==n?"ok   ":"*bad*",
	     s);
      
      if(i!=n)
	c++;
    }
  printf("Really used: %d\n",t);
  printf("Collisions: %d = %1.3f%%\n",
	 c,
	 100.0*c/(double)t);
}
