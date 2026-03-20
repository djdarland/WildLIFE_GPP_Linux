#define EXTERN extern
#include "defs.h"
void RefListHeader::List_SetLinkProc (RefListGetLinksProc getLinks)
// RefListHeader header;
// RefListGetLinksProc getLinks;
{
  wl_ListHeader *header;

  header = (wl_ListHeader*) this;
  
  header->First = NULL;
  header->Last = NULL;
#ifdef prlDEBUG
  header->Lock = 0;
#endif
  header->GetLinks = getLinks;
}
/*=============================================================================*/
/*			List functions					       */
/*=============================================================================*/
void RefListHeader::List_InsertAhead (Ref atom) 
// RefListHeader header;
// Ref atom;
{
  wl_ListHeader *header;

  header = (wl_ListHeader*) this;
  
  RefListGetLinksProc  getLinks = ((wl_ListHeader*)header)->GetLinks;
  /* Update links of atom to insert */
  (*getLinks)(atom)->Next = ((wl_ListHeader*)header)->First;
  (*getLinks)(atom)->Prev = NULL;
  /* Link to the head of list */
  if (((wl_ListHeader*)header)->First != NULL)
    (*getLinks)(((wl_ListHeader*)header)->First)->Prev = atom;
  else	/* The list is empty */
    ((wl_ListHeader*)header)->Last  = atom;
  ((wl_ListHeader*)header)->First = atom;
}
/*==============================================================================*/
void RefListHeader::List_Append (Ref atom) 
// RefListHeader header;
// Ref atom;
{
  wl_ListHeader *header;

  header = (wl_ListHeader*) this;
  
  RefListGetLinksProc  getLinks = ((wl_ListHeader*)header)->GetLinks;
  /* Link to the end of list */
  if (((wl_ListHeader*)header)->Last != NULL)
    (*getLinks)(((wl_ListHeader*)header)->Last)->Next = atom;
  else	/* The list is empty */
    ((wl_ListHeader*)header)->First = atom;
  /* Update links of atom to insert */
  (*getLinks)(atom)->Prev = ((wl_ListHeader*)header)->Last;
  (*getLinks)(atom)->Next = NULL;
  /* Update last element of header */
  ((wl_ListHeader*)header)->Last  = atom;
}
/*==============================================================================*/
void RefListHeader::List_InsertBefore (Ref atom, Ref mark)
// RefListHeader header;
// Ref atom;
// Ref mark;
{
  wl_ListHeader *header;

  header = (wl_ListHeader*) this;
  
  RefListGetLinksProc  getLinks = ((wl_ListHeader*)header)->GetLinks;

  if (mark != NULL)
    {
      (*getLinks)(atom)->Next = mark;
      if (mark != ((wl_ListHeader*)header)->First)
        {
	  (*getLinks)(atom)->Prev = (*getLinks)(mark)->Prev;
	  (*getLinks)((*getLinks)(mark)->Prev)->Next = atom;
        }
      else	/* Insert ahead the list */
        {
	  (*getLinks)(atom)->Prev = NULL;
	  ((wl_ListHeader*)header)->First = atom;
        }
      (*getLinks)(mark)->Prev = atom;
    }
  else        /* Append to the list */
    ((RefListHeader*)header)->List_Append (atom);
} 
/*==============================================================================*/
void RefListHeader::List_InsertAfter (Ref atom, Ref mark)
// RefListHeader header;
// Ref atom;
// Ref mark;
{
  wl_ListHeader *header;

  header = (wl_ListHeader*) this;
  
  RefListGetLinksProc  getLinks = ((wl_ListHeader*)header)->GetLinks;

#ifdef prlDEBUG
  if (header->Lock > 1)
    OS_PrintMessage ("List_InsertAfter: Warning insert after on recursive List_Enum call !!\n");
#endif
  if (mark != NULL)
    {
      (*getLinks)(atom)->Prev = mark;
      if (mark != ((wl_ListHeader*)header)->Last)
        {
	  (*getLinks)(atom)->Next = (*getLinks)(mark)->Next;
	  (*getLinks)((*getLinks)(mark)->Next)->Prev = atom;
        }
      else	/* Insert at the end of the list */
        {
	  (*getLinks)(atom)->Next = NULL;
	  ((wl_ListHeader*)header)->Last = atom;
        }
      (*getLinks)(mark)->Next = atom;
    }
  else        /* Insert ahead the list */
    ((RefListHeader*)header)->List_InsertAhead (atom);
} 
/*==============================================================================*/
void RefListHeader::List_Swap (Ref first, Ref second)
// RefListHeader header;
// Ref first;
// Ref second;
{
  wl_ListHeader *header;

  header = (wl_ListHeader*) this;
  
  RefListGetLinksProc	getLinks = ((wl_ListHeader*)header)->GetLinks;
  /* Don't swap if the input is wrong */
  if ((*getLinks)(first)->Next != second)
    {
#ifdef prlDEBUG
      OS_PrintMessage ("List_Swap: WARNING wrong input data, swap not done..\n");
#endif
      return;
    }
  /* Special Cases */
  if (((wl_ListHeader*)header)->First == first)
    ((wl_ListHeader*)header)->First = second;
  else
    (*getLinks)((*getLinks)(first)->Prev)->Next = second;
  if (((wl_ListHeader*)header)->Last == second)
    ((wl_ListHeader*)header)->Last = first;
  else
    (*getLinks)((*getLinks)(second)->Next)->Prev = first;
  /* Swap the atoms */
  (*getLinks)(second)->Prev = (*getLinks)(first)->Prev;
  (*getLinks)(first)->Next  = (*getLinks)(second)->Next;
  (*getLinks)(first)->Prev  = second;
  (*getLinks)(second)->Next = first;
}
/*==============================================================================*/
long long RefListHeader::List_SwapLinks (Ref atom)
// RefListHeader header;
// Ref atom;
{
  Ref	save;

  wl_ListHeader *header;

  header = (wl_ListHeader*) this;

  //  save = (((wl_ListHeader*)*header)->GetLinks)(atom)->Next;
  save = (((wl_ListHeader*)header)->GetLinks)(atom)->Next;
  //  (*header->GetLinks)(atom)->Next = (((wl_ListHeader*)*header)->GetLinks)(atom)->Prev;
  (header->GetLinks)(atom)->Next = (((wl_ListHeader*)header)->GetLinks)(atom)->Prev;
  //  (*header->GetLinks)(atom)->Prev = save;
    (header->GetLinks)(atom)->Prev = save;
  return TRUE;
}
void RefListHeader::List_Reverse ()
// RefListHeader header;
{
  Ref			cur, next;

  wl_ListHeader *header;

  header = (wl_ListHeader*) this;

  RefListGetLinksProc	getLinks = header->GetLinks;
  /* This traverse cannot be done with function List_Enum() */
  cur = header->First;
  /* Swap the headers */
  header->First = header->Last;
  header->Last  = cur;
  while (cur != NULL)
    {
      next = (*getLinks)(cur)->Next;
      ((RefListHeader*)header)->List_SwapLinks (cur);
      cur = next;
    }
}
/*==============================================================================*/
void RefListHeader::List_Remove (Ref atom)
// RefListHeader header;
// Ref atom;
{
  /*-----------------------------------------------------------------------------
    WARNING
    - The container is 'updated' two times if the first and last atom
    of list is the only one to remove.
    -----------------------------------------------------------------------------*/
  wl_ListHeader *header;

  header = (wl_ListHeader*) this;

  RefListGetLinksProc  getLinks = header->GetLinks;

#ifdef prlDEBUG
  if (header->Lock > 1)
    OS_PrintMessage ("List_Remove: Warning remove on recursive List_Enum call !!\n");
#endif
  /* Update the DownStream links */
  if ((*getLinks)(atom)->Prev != NULL)
    {
      (*getLinks)((*getLinks)(atom)->Prev)->Next = 
	(*getLinks)(atom)->Next;
    }
  else            /* Atom is the first of list */
    header->First = (*getLinks)(atom)->Next;
  /* Update the UpStream links */
  if ((*getLinks)(atom)->Next != NULL)
    {
      (*getLinks)((*getLinks)(atom)->Next)->Prev = 
	(*getLinks)(atom)->Prev;
    }
  else            /* Atom is the last of list */
    header->Last = (*getLinks)(atom)->Prev;
  /* Reset the atom links */
  (*getLinks)(atom)->Prev = NULL;
  (*getLinks)(atom)->Next = NULL;
} 
/*==============================================================================*/
void RefListHeader::List_Concat (wl_ListHeader *header2)
// RefListHeader header1;
// RefListHeader header2;
{
  wl_ListHeader *header1;

  header1 = (wl_ListHeader*) this;
  
  RefListGetLinksProc  getLinks = ((wl_ListHeader*)header1)->GetLinks;
  if (((wl_ListHeader*)header1)->GetLinks == ((wl_ListHeader*)header2)->GetLinks)
    {
#ifdef prlDEBUG
      OS_PrintMessage ("List_Concat: ERROR concat different lists\n");
#endif
      return;
    }
  /* Concatenate only if the second list is not empty */
  if (header2->First != NULL)
    {
      /* Obvious concatenate when the first list is empty */
      if (header1->First == NULL)
	header1->First = header2->First;

      else	/* Concatenate the two non empty lists */
        {
	  (*getLinks)(header1->Last)->Next  = header2->First;
	  (*getLinks)(header2->First)->Prev = header1->Last;
        }
      header1->Last = header2->Last;
    }
} 
/*==============================================================================*/
long long RefListHeader::List_EnumFrom (Ref atom,
			 RefListEnumProc proc, Ref closure)
// RefListHeader	header;
// Ref atom;
// RefListEnumProc	proc;
// Ref closure;
{
  Ref	cur, next;
  wl_ListHeader *header;

  header = (wl_ListHeader*) this;
  
  int	notInterrupted = TRUE;

#ifdef prlDEBUG
  header->Lock += 1;
#endif
  cur = atom;
  while (cur != NULL && notInterrupted)
    {
      next = List_Next (header, cur);
      notInterrupted = (*proc)(cur, closure);
      cur = next;
    }
#ifdef prlDEBUG
  header->Lock -=1;
#endif
  return (notInterrupted);
}
/*==============================================================================*/
long long RefListHeader::List_Enum (RefListEnumProc proc, Ref closure)
// RefListHeader	header;
// RefListEnumProc	proc;
// Ref closure;
/*-----------------------------------------------------------------------------
  (NO) SIDE EFFECTS
  The current atom can be modified by the function RemoveAtom () during
  the traversing of the list. This is the reason why the current pointer
  is managed on the header.
  -----------------------------------------------------------------------------*/
{
  wl_ListHeader *header;

  header = (wl_ListHeader*) this;
  
  //  return (List_EnumFrom (header, ((wl_ListHeader*)header)->First, proc, closure));
  return ((RefListHeader*)header)->List_EnumFrom(((wl_ListHeader*)header)->First, proc, closure);
}
/*==============================================================================*/
long long RefListHeader::List_EnumBackFrom (Ref atom,
			     RefListEnumProc proc, Ref closure)
// RefListHeader	header;
// Ref		atom;
// RefListEnumProc	proc;
// Ref		closure;
{
  Ref	cur, prev;
  int	notInterrupted = TRUE;
  wl_ListHeader *header;

  header = (wl_ListHeader*) this;

#ifdef prlDEBUG
  header->Lock += 1;
#endif
  cur = atom;
  while (cur != NULL && notInterrupted)
    {
      prev = List_Prev (header, cur);
      notInterrupted = (*proc)(cur, closure);
      cur = prev;
    }
#ifdef prlDEBUG
  header->Lock -=1;
#endif
  return (notInterrupted);
}
/*==============================================================================*/
long long RefListHeader::List_EnumBack (RefListEnumProc proc, Ref closure)
// RefListHeader	header;
// RefListEnumProc	proc;
// Ref			closure;
{
  wl_ListHeader *header;

  header = (wl_ListHeader*) this;
  
  return (((RefListHeader*)header)->List_EnumBackFrom ((wl_ListHeader*)header->Last, proc, closure));
}

long long RefListHeader::List_Card ()
// RefListHeader header;
{
  long long n = 0;
  wl_ListHeader *header;

  header = (wl_ListHeader*) this;
    
  ((RefListHeader*)header)->List_Enum((RefListEnumProc) List_CountAtom, &n); // REV401PLUS cast
  return n;
}

void RefListHeader::List_Cut (Ref atom, wl_ListHeader *newHeader)
// RefListHeader	header;
// Ref			atom;
// RefListHeader	newHeader;
{
  wl_ListHeader *header;

  header = (wl_ListHeader*) this;
    

  RefListGetLinksProc  getLinks = ((wl_ListHeader*)header)->GetLinks;

  if (atom != List_Last (header))
    {
      ((wl_ListHeader*)newHeader)->First = List_Next (((wl_ListHeader*)header), atom);
      ((wl_ListHeader*)newHeader)->Last  = ((wl_ListHeader*)header)->Last;
      ((wl_ListHeader*)header)->Last = atom;
      /* Update the links */
      (*getLinks)(atom)->Next = NULL;
      (*getLinks)(newHeader->First)->Prev = NULL;
    }
}
/*==============================================================================*/
long long List_CountAtom (Ref p, Ref nbR)
// Ref p; 
// Ref nbR;
{
  long long *nb = (long long *)nbR;
    
  ++*nb;
  return TRUE;
}

