#pragma once
#include "defs.h"
class RefListHeader {
  public:
  //  Ref First, Last;
#ifdef prlDEBUG
  // Int32			Lock;
#endif
  // RefListGetLinksProc		GetLinks;

  void List_SetLinkProc (RefListGetLinksProc getLinks);
  void List_InsertAhead (Ref atom);
  void List_Append (Ref atom);
  void List_InsertBefore (Ref atom, Ref mark);
  void List_InsertAfter (Ref atom, Ref mark);
  void List_Swap (Ref first, Ref second);
  long long List_SwapLinks (Ref atom);
  void List_Reverse ();
  void List_Remove (Ref atom);
  void List_Concat (wl_ListHeader *header2);
  long long List_EnumFrom (Ref atom,
			   RefListEnumProc proc, Ref closure);
  long long List_Enum (RefListEnumProc proc, Ref closure);
  long long List_EnumBackFrom (Ref atom,
			       RefListEnumProc proc, Ref closure);
  long long List_EnumBack (RefListEnumProc proc, Ref closure);
  long long List_Card ();
  void List_Cut (Ref atom, wl_ListHeader *newHeader);

};

long long List_CountAtom (Ref p, Ref nbR);
