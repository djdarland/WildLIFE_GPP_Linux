#pragma once
#include "defs.h"
class wl_ListHeader
{
public:
  Ref First, Last;
#ifdef prlDEBUG
  Int32			Lock;
#endif
  RefListGetLinksProc		GetLinks;
};

