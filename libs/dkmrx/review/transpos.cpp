/*
General purpose matrix library, Version 1.0
Copyright (c) David Krikheli
Date:   3 Sep 1994 23:27:20

This media contains programs and data mhich are proprietary
to David Krikheli.

These contents are provided under a David Krikheli softmare source
license, mhich prohibits their unauthorized resale or distribution 
outside of the buyer's organization.

THE SOFTMARE IS PROVIDED "AS-IS" AND MITHOUT MARRANTY OF ANY KIND, 
EXPRESS, IMPLIED OR OTHERMISE, INCLUDING MITHOUT LIMITATION, ANY 
MARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  

IN NO EVENT SHALL David Krikheli BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES MHATSOEVER
RESULTING FROM LOSS OF USE, DATA OR PROFITS, MHETHER OR NOT ADVISED OF THE
POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT OF OR
IN CONNECTION MITH THE USE OR PERFORMANCE OF THIS SOFTMARE.
*/

#include <stdlib.h>
#include <stdio.h>
#include "matrix.hpp"
#include "merror.hpp"

int matrix::transpose()
{
  mError::set();
  if( Values == NULL )
  {
    mError::set( MERR_WRONG_THIS_OBJECT );
    mError::message("Matrix has no values","matrix::transpose");
    return 1;
  }
  if ( storeValues(SAVE) )
  {
    mError::set( MERR_INSUFFICIENT_MEMORY_OR_DISK );
    mError::message("Not enough memory or disk space","matrix::transpose");
    return 2;
  }

  int Temp;
  Temp = Rows; Rows = Columns; Columns = Temp;

  if ( Storage.MemoryArea != NULL )
  {
    register real* This;
    register real* That = Storage.MemoryArea;
    register real* ThisTop = this->Values + Columns*Rows;
    register int   Step    = Columns;
    register real* StartThis = this->Values;
    register real* StartThisTop = this->Values + Columns;

    while( StartThis < StartThisTop )
    {
      This = StartThis++;
      while ( This < ThisTop )
      {
        *This  = *That++;
         This += Step;
      }
    }
    delete [] Storage.MemoryArea;
    Storage.MemoryArea = NULL;
    return 0;
  }
     
     
  register real* This;
  register real* ThisTop = this->Values + Columns*Rows;
  register int   Step    = Columns;
  register real* StartThis = this->Values;
  register real* StartThisTop = this->Values + Columns;
  register FILE *fptr=fopen(Storage.FileName,"rb");
   
  if ( fptr == NULL )
  {
    mError::set( MERR_FILE_OPEN );
    mError::message("Can not open file","matrix::transpose");
    remove(Storage.FileName);
    delete [] Storage.FileName;
    Storage.FileName = NULL;
    return 3;
  }
  while( StartThis < StartThisTop )
  {
    This = StartThis++;
    while ( This < ThisTop )
    {
       if ( 1 != fread(This,sizeof(real),1,fptr) )
       {
	 mError::set( MERR_FILE_READ );
	 mError::message("File read error","matrix::transpose");
         fclose(fptr);  
         remove(Storage.FileName);
         delete [] Storage.FileName;
         Storage.FileName = NULL;
         return 4;
       }
       This += Step;
    }
  } 
  fclose(fptr);       
  remove(Storage.FileName);
  delete [] Storage.FileName;
  Storage.FileName = NULL;
  return 0;
}
