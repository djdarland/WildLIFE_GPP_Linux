#define EXTERN extern
#include "defs.h"
/******** UPDATE_SYMBOL(m,s)
S is a string of characters encountered during parsing, M is the module it
belongs too.

if M is NULL then extract the module name from S. If that fails then use the
current module.
  
Then, retrieve the keyword for 'module#symbol'. Then find the correct
definition by scanning the opened modules.
*/
ptr_definition wl_module_ptr::update_symbol(char *symbol)   /*  RM: Jan  8 1993  */
//     ptr_module module;
//     char *symbol;
{
  ptr_keyword key;
  ptr_definition result=NULL;
  ptr_int_list opens;
  ptr_module opened;
  ptr_keyword openkey;
  ptr_keyword tempkey;
  ptr_module module;

  dbg_top("update_symbol");
  module = (ptr_module) this;
  if(djd){fprintf(dbg,"symbol = %s\n",symbol); fflush(dbg);}
  if(djd){fprintf(dbg,"module = %p\n",module); fflush(dbg);}
  /* First clean up the arguments and find out which module to use */
  if(!module || (module == nill_module)) {
    module=extract_module_from_name(symbol);  // need to check return DJD
    if(!module || (module == nill_module))
      module=current_module;
    symbol=strip_module_name(symbol);
  }
  /* Now look up 'module#symbol' in the symbol table */
  if (module->symbol_table)
    key=((wl_hash_table_ptr*)module->symbol_table)->hash_lookup(symbol);
  else key = NULL;
  if(key)
    if(key->wl_public || module==current_module)
      result=key->definition;
    else {
      Errorline("qualified call to private symbol '%s'\n",
		key->combined_name);
      result=error_psi_term->type;
    }
  else
    if(module!=current_module) {
      Errorline("qualified call to undefined symbol '%s#%s'\n",
		module->module_name,symbol);
      result=error_psi_term->type;
    }
    else
      {
	/* Add 'module#symbol' to the symbol table */
	key=HEAP_ALLOC(struct wl_keyword);
	key->module=module;
	key->symbol=heap_copy_string(symbol);
	key->combined_name=heap_copy_string(((wl_module_ptr*)module)->make_module_token(symbol));
	key->wl_public=FALSE;
	key->private_feature=FALSE; /*  RM: Mar 11 1993  */
	key->definition=NULL;
	if (module->symbol_table)
	  ((wl_hash_table_ptr*)module->symbol_table)->hash_insert(key->symbol,key);
	/* Search the open modules of 'module' for 'symbol' */
	opens=module->open_modules;
	openkey=NULL;
	while(opens) {
	  opened=(ptr_module)(opens->value_1);
	  if(opened!=module) {
	    if (opened->symbol_table)
	      tempkey=((wl_hash_table_ptr*)opened->symbol_table)->hash_lookup(symbol);
	    else tempkey = NULL;
	    if(tempkey)
	      if(openkey && openkey->wl_public && tempkey->wl_public) {
		if(openkey->definition==tempkey->definition) {
		  Warningline("benign module name clash: %s and %s\n",
			      openkey->combined_name,
			      tempkey->combined_name);
		}
		else {
		  Errorline("serious module name clash: \"%s\" and \"%s\"\n",
			    openkey->combined_name,
			    tempkey->combined_name);
		  
		  result=error_psi_term->type;
		}
	      }
	      else
		if(!openkey || !openkey->wl_public)
		  openkey=tempkey;
	  }
	  opens=opens->next;
	}
	if(!result) { /*  RM: Feb  1 1993  */
	  if(openkey && openkey->wl_public) {
	    /* Found the symbol in an open module */
	    if(!openkey->wl_public)
	      Warningline("implicit reference to non-public symbol: %s\n",
			  openkey->combined_name);
	    result=openkey->definition;
	    key->definition=result;
	  }
	  else { /* Didn't find it */
	    result=new_definition(key);
	  }
	}
      }
  return result;
}

/********* UPDATE_FEATURE(module,feature)
	   Look up a FEATURE.
	   May return NULL if the FEATURE is not visible from MODULE.
*/
ptr_definition wl_module_ptr::update_feature(char *feature)
//     ptr_module module;
//     char *feature;
{
  ptr_keyword key;
  ptr_module wl_explicit;
  ptr_module module;

  module = (ptr_module) this;
  /* Check if the feature already contains a module name */
  if(module == nill_module)
    module=current_module;
  wl_explicit=extract_module_from_name(feature);
  if(wl_explicit)
    if(wl_explicit!=module)
      return NULL; /* Feature isn't visible */
    else
      return ((wl_module_ptr*)nill_module)->update_symbol(feature);
  /* Now we have a simple feature to look up */
  if (module->symbol_table)
    key=((wl_hash_table_ptr*)module->symbol_table)->hash_lookup(feature);
  else key = NULL;
  if(key && key->private_feature)
    return key->definition;
  else
    return ((wl_module_ptr*)module)->update_symbol(feature);
}


/******************************************************************************
  
  Here are the routines which allow a new built_in type, predicate or function
  to be declared.
  
****************************************************************************/
/******** NEW_BUILT_IN(m,s,t,r)
	  Add a new built-in predicate or function.
	  Used also in x_pred.c

	  M=module.
	  S=string.
	  T=type (function or predicate).
	  R=address of C routine to call.
*/
void wl_module_ptr::new_built_in(char *s,def_type t,long long (*r)())
//     ptr_module m;
//     char *s;
//     def_type t;
//     long long (*r)();
{
  ptr_definition d;
  ptr_module m;

  m = (ptr_module) this;
  
  if (built_in_index >= MAX_BUILT_INS) {
    fprintf(stderr,"Too many primitives, increase MAX_BUILT_INS in extern.h\n");
    exit(-1);
  }

  if(m!=current_module)  /*  RM: Jan 13 1993  */
    set_current_module(m);
  d=((wl_module_ptr*)m)->update_symbol(s); /* RM: Jan  8 1993 */
  d->type_def=t;
  built_in_index++;
  d->rule=(ptr_pair_list )built_in_index;
  c_rule[built_in_index]=r;
}

/******** MAKE_MODULE_TOKEN(module,string)
Write 'module#string' in module_buffer.
If string is a qualified reference to a given module, then modify the calling
module variable to reflect this.

The result must be immediately stored in a newly allocated string.
*/
char *wl_module_ptr::make_module_token(char *str)
//     ptr_module module;
//     char *str;
{
  ptr_module wl_explicit;
  ptr_module module;

  module = (ptr_module) this;
  
  /* Check if the string already contains a module */
  wl_explicit=extract_module_from_name(str);
  if(wl_explicit)
    strcpy(module_buffer,str);
  else
    if(module!=no_module) {
      strcpy(module_buffer,module->module_name);
      strcat(module_buffer,"#");
      strcat(module_buffer,str);
    }
    else
      strcpy(module_buffer,str);
  return module_buffer;
}

