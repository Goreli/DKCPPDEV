/*
MIT License

Copyright(c) 2019 David Krikheli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
/*
Modification history:

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
