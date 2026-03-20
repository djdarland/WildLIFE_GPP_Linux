/* 
 * Copyright 1991 Digital Equipment Corporation.
 * All Rights Reserved.
 */
/* 	$Id: list.c,v 1.2 1994/12/08 23:28:16 duchier Exp $	 */
/*
** list.c contains the functions to manage double link list
** with 2 entries (first and last element)
** Links belong longs to each atom
*/
#define EXTERN extern
#define REV401PLUS
#ifdef REV401PLUS
#include "defs.h"
#endif
/*=============================================================================*/
/*			Set functions					       */
/*=============================================================================*/
/*==============================================================================*/
long long List_IsUnlink (RefListLinks links)
// RefListLinks links;
{
  return (links->Next == NULL && links->Prev == NULL);
}
/*==============================================================================*/
