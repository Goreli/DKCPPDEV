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

#include "dkmrx_matrix.hpp"
#include "dkmrx_error.hpp"

using namespace dkmrx;

/*
Could be usefull:

matrix& getHG(matrix&);
matrix& getNonHG(matrix&);
*/

int matrix::toHG(real scaleFactor)    // to HomoGeneous form
{
  mError::set();
  if ( pValues_ == nullptr ) 
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 0 );
    mError::message("Can not apply to an EMPTY object","matrix::toHG(real)");
    return 2;
  }
  real* newval = new real [(iColumns_+1) * iRows_];
  if ( newval == nullptr)
  {
    mError::set( MERR_INSUFFICIENT_MEMORY );
    mError::message("Not enough memory","matrix::toHG(real)");
    return 3;
  }

  real* Target=newval;
  real* Source=pValues_;
  real* sourceBorder=Source+iColumns_;
  real* sourceTop=Source+iColumns_*iRows_;
  int step=iColumns_;

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
        
  delete [] pValues_;
  pValues_ = newval;
  iColumns_++;
  return 0;
}

int matrix::fromHG()    // from HomoGeneous form
{
  mError::set();
  if ( pValues_ == nullptr)
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 0 );
    mError::message("Can not apply to an EMPTY object","matrix::fromHG()");
    return 2;
  }
  if ( iColumns_ == 1 ) 
  {
    mError::set( MERR_WRONG_THIS_OBJECT, 1 );
    mError::message("Can not apply to a 1 column object","matrix::fromHG()");
    return 3;
  }
  real* newval = new real [(iColumns_-1) * iRows_];
  if ( newval == nullptr)
    return 4;
  
  int step=iColumns_-1;
  real* Target=newval;
  real* Source=pValues_;
  real* targetBorder=Target+step;
  real* sourceTop=pValues_+iColumns_*iRows_;
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

  --iColumns_;
  delete [] pValues_;
  pValues_ = newval;
  return 0;
}
