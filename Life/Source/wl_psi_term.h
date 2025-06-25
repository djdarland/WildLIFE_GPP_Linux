#pragma once
#include "defs.h"
/* PSI_TERM */
class wl_psi_term {
public:
#ifdef TS
  unsigned long long time_stamp; /* Avoid multiple trailing on a choice point. 9.6 */
#endif
  ptr_definition type;
  long long status; /* Indicates whether the properties of the type have been */
  /* checked or the function evaluated */
  /* long long curried; Distinguish between quoted and curried object 20.5 */
  long long flags; /* 14.9 */
  GENERIC value_3;
  ptr_node attr_list;  // For classes DJD
  ptr_psi_term coref;
  ptr_residuation resid; /* List of goals to prove if type is narrowed. */
};
