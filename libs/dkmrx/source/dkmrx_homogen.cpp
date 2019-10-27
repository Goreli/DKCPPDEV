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

#include "dkmrx_matrix.hpp"
#include <stdlib.h>
#include <math.h>
#include "dkmrx_error.hpp"

/*
Could be usefull:

matrix& getHG(matrix&);
matrix& getNonHG(matrix&);
*/

int matrix::toHG(real scaleFactor)    // to HomoGeneous form
{
  mError::set();
  if ( Status == STATUS::TEMPORARY )
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 3 );
    mError::message("Can not apply to a TEMPORARY object","matrix::toHG");
    delete  this;
    return 1;
  }
  if ( Values == NULL ) 
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 0 );
    mError::message("Can not apply to an EMPTY object","matrix::toHG");
    return 2;
  }
  real* newval = new real [(Columns+1) * Rows];
  if ( newval == NULL)
  {
    mError::set( MERR_INSUFFICIENT_MEMORY );
    mError::message("Not enough memory","convertToHG");
    return 3;
  }

  real* Target=newval;
  real* Source=Values;
  real* sourceBorder=Source+Columns;
  real* sourceTop=Source+Columns*Rows;
  int step=Columns;

  if ( scaleFactor == (real) 1 ) 
    while ( sourceBorder <= sourceTop )
    {
      while ( Source < sourceBorder )  *Target++ = *Source++;
      *Target++ = (real) 1;
      sourceBorder += step;
    }
  else
    while ( sourceBorder <= sourceTop )
    {
      while ( Source < sourceBorder )  *Target++ = scaleFactor * *Source++;
      *Target++ = scaleFactor;
      sourceBorder += step;
    }
        
  delete [] Values;
  Values = newval;
  Columns++;
  return 0;
}

int matrix::fromHG()    // from HomoGeneous form
{
  mError::set();
  if ( Status == STATUS::TEMPORARY )
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 3 );
    mError::message("Can not apply to a TEMPORARY object","matrix::fromHG");
    delete this;
    return 1;
  }
  if ( Values == NULL ) 
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 0 );
    mError::message("Can not apply to an EMPTY object","matrix::fromHG");
    return 2;
  }
  if ( Columns == 1 ) 
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 1 );
    mError::message("Can not apply to a 1 column object","matrix::fromHG");
    return 3;
  }
  real* newval = new real [(Columns-1) * Rows];
  if ( newval == NULL)
  {
    mError::set( MERR_INSUFFICIENT_MEMORY );
    mError::message("Not enough memory","convertToHG");
    return 4;
  }

  int step=Columns-1;
  real* Target=newval;
  real* Source=Values;
  real* targetBorder=Target+step;
  real* sourceTop=Values+Columns*Rows;
  real scaleFactor;

  while ( Source < sourceTop )
  { // Assumed that none of vectors has a scale factor equal to zero
    scaleFactor = *(Source+step);
    while ( Target < targetBorder )
    {  
      *Target++ = *Source++/scaleFactor;
       //Target++;
       //Source++;
    }
    targetBorder += step;
    Source++;
  }

  --Columns;
  delete [] Values;
  Values = newval;
  return 0;
}
