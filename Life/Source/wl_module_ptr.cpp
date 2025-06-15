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

  module = (ptr_module) this;
  
  /* First clean up the arguments and find out which module to use */
  if(module == nill_module) {
    module=extract_module_from_name(symbol);  // need to check return DJD
    if(module == nill_module)
      module=current_module;
    symbol=strip_module_name(symbol);
  }
  /* Now look up 'module#symbol' in the symbol table */
  key=hash_lookup(module->symbol_table,symbol);
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
	key->combined_name=heap_copy_string(make_module_token(module,symbol));
	key->wl_public=FALSE;
	key->private_feature=FALSE; /*  RM: Mar 11 1993  */
	key->definition=NULL;
	hash_insert(module->symbol_table,key->symbol,key);
	/* Search the open modules of 'module' for 'symbol' */
	opens=module->open_modules;
	openkey=NULL;
	while(opens) {
	  opened=(ptr_module)(opens->value_1);
	  if(opened!=module) {
	    tempkey=hash_lookup(opened->symbol_table,symbol);
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
  key=hash_lookup(module->symbol_table,feature);
  if(key && key->private_feature)
    return key->definition;
  else
    return ((wl_module_ptr*)module)->update_symbol(feature);
}
