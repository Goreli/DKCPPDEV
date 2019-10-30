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

#include <cstring>
#include <cstdio>
#include "dkmrx_matrix.hpp"    
#include "dkmrx_error.hpp"

using namespace dkmrx;

#if defined(MATRIX_DEBUG)
int	matrix::Created = 0;
int	matrix::Destroied = 0;
#endif

matrix::matrix(void)
{
#if defined(MATRIX_DEBUG)
	Created++;
#endif
	Rows=Columns=0;
	Status=STATUS::PERMANENT;
	Storage.MemoryArea = nullptr;
	Storage.FileName   = nullptr;
	Values= nullptr;
	Name= nullptr;
}

matrix::matrix(int rows,int columns)
{
#if defined(MATRIX_DEBUG)
	Created++;
#endif
	mError::set();
	Rows=rows;  Columns=columns;
	Status= STATUS::PERMANENT;
	Storage.MemoryArea = nullptr;
	Storage.FileName   = nullptr;
	Values= nullptr;
	Name= nullptr;

	if ( (Rows<=0) || (Columns<=0) )
	{
		if ( !(Rows==0 && Columns==0) )
		{
			mError::set( MERR_ILLEGAL_DIMENSION );
			mError::message("Illegal dimension","matrix::matrix(int, int)");
		}
		Rows=Columns=0;
	}
	else
	{
		size_t MatSize=Rows * Columns;
		Values= new real[MatSize];
		if ( Values == nullptr)
		{
			Rows=Columns=0;
			mError::set( MERR_INSUFFICIENT_MEMORY );
			mError::message("Not enough memory","matrix::matrix(int, int)");
		}
	}
}

matrix::matrix(real init_value,int rows,int columns)
{
#if defined(MATRIX_DEBUG)
	Created++;
#endif
	mError::set();
	Rows=rows;  Columns=columns;
	Status= STATUS::PERMANENT;
	Storage.MemoryArea = nullptr;
	Storage.FileName   = nullptr;
    Values= nullptr;
	Name= nullptr;

	if ( (Rows<=0) || (Columns<=0) )
	{
		if ( !(Rows==0 && Columns==0) )
		{
			mError::set( MERR_ILLEGAL_DIMENSION );
			mError::message("Illegal dimension","matrix::matrix(real, int, int)");
		}
		Rows=Columns=0;
	}
	else
	{
		size_t MatSize=Rows * Columns;
		Values= new real[MatSize];
		if ( Values == nullptr)
		{
			Rows=Columns=0;
			mError::set( MERR_INSUFFICIENT_MEMORY );
			mError::message("Not enough memory","matrix::matrix(real, int, int)");
		}
		else
		{
		real *index,*top;

			index = top = Values;
			top  += MatSize;
			while( index<top ) *index++ = init_value;
		}
	}
}

matrix::matrix(matrix& m)
{
#if defined(MATRIX_DEBUG)
	Created++;
#endif
	Rows=0;  Columns=0;
	Status= STATUS::PERMANENT;
	Storage.MemoryArea = nullptr;
	Storage.FileName   = nullptr;
	Values= nullptr;
	Name= nullptr;

	*this=m;
}

matrix::~matrix()
{
#if defined(MATRIX_DEBUG)
	Destroied++;
#endif
	if (Values != nullptr) delete [] Values;
	if (Name   != nullptr) delete [] Name;
}

void matrix::empty()
{
	Rows=Columns=0;
	if ( Values != nullptr)
	{
		delete [] Values;
		Values= nullptr;
	}
}

int matrix::storeValues(STORE_OPERATION So)
{   
	mError::set();
  size_t AreaSize = Rows*Columns;

  if( So == STORE_OPERATION::SAVE )
  { 
    if (
	( Storage.MemoryArea != nullptr) ||
	( Storage.FileName   != nullptr)
       ) 
    {   
      mError::set( MERR_VALUES_ALREADY_SAVED );
      mError::message("values have already been saved","matrix::storeValues");
      return 0;
    }                                          
    real* Tempreal = new real[ AreaSize ];
    if ( Tempreal != nullptr)
    {
      std::memcpy(Tempreal,Values,AreaSize*sizeof(real));
      Storage.MemoryArea = Values; 
      Values = Tempreal;
      return 0;
    }
  
    Storage.FileName = new char[L_tmpnam+1];
    if ( Storage.FileName == nullptr)
    {
      mError::set( MERR_INSUFFICIENT_MEMORY );
      mError::message("Not enough memory","matrix::storeValues");
      return 1;
    } 
	std::tmpnam( Storage.FileName );
	std::FILE* Fptr = std::fopen(Storage.FileName,"mb");
    if ( Fptr != nullptr)
    { 
      if ( AreaSize == std::fwrite(Values,sizeof(real),AreaSize,Fptr ) )
      { 
		  std::fclose(Fptr);
	return 0;
      }
	  std::fclose(Fptr);
	  std::remove( Storage.FileName );
    }    
    mError::set( MERR_FILE_WRITE );
    mError::message("Unable to mrite to a file","matrix::storeValues");
    delete [] Storage.FileName;
    Storage.FileName = nullptr;
    return 1;
  }               
  
// Else So == LOAD 
  if( So == STORE_OPERATION::LOAD )
  { 
    if (
	( Storage.MemoryArea == nullptr) &&
	( Storage.FileName   == nullptr)
       ) 
    {   
      mError::set( MERR_VALUES_NOT_LOADED );
      mError::message("values have not been loaded","matrix::storeValues");
      return 0;
    }                                          
    if ( Storage.MemoryArea != nullptr)
    { 
      delete [] Values;
      Values = Storage.MemoryArea; 
      Storage.MemoryArea = nullptr;
      return 0;
    } 
    // So ..., Storage.FileName != nullptr
	std::FILE* Fptr = std::fopen(Storage.FileName,"rb");
    if ( Fptr != nullptr)
    {
      if ( AreaSize == std::fread( Values,sizeof(real),AreaSize,Fptr ) )
      { 
		  std::fclose( Fptr );
		  std::remove(Storage.FileName);
	delete [] Storage.FileName;
	Storage.FileName = nullptr;
	return 0;
      } 
	  std::fclose(Fptr);
    }  
	std::remove(Storage.FileName);
    delete [] Storage.FileName;
    Storage.FileName = nullptr;
    mError::set( MERR_FILE_READ );
    mError::message("Unable to read from a file","matrix::storeValues");
    return 1;
  }               
return 1;             
}                        

matrix& matrix::identity(int Dim)
{
	mError::set();
  if ( Dim <= 0 )
  {
    mError::set( MERR_ILLEGAL_DIMENSION );
    mError::message("Illegal dimension","matrix::identity");
    matrix *I = new matrix;
    I->Status = STATUS::TEMPORARY;
    return *I;
  }

matrix *I = new matrix(0.0, Dim, Dim);
  if ( I->Values == nullptr)
  {
	mError::set( MERR_INSUFFICIENT_MEMORY );
	mError::message("Not enough memory","matrix::identity");
  }
  else
  {
    int   step=Dim + 1;
    real* Ptr;
    real* PtrTop=I->Values + Dim*Dim;
    for ( Ptr=I->Values; Ptr < PtrTop; Ptr += step ) *Ptr = 1.0;
  }
  I->Status = STATUS::TEMPORARY;
  return *I;
}

void matrix::name(const char* N)
{                     
  mError::set();
  delete [] Name;
  if ( N == nullptr) Name = nullptr;
  else
  {
    Name = new char [ strlen(N) + 1 ];
    if ( Name == nullptr)
    {
	mError::set( MERR_INSUFFICIENT_MEMORY );
	mError::message("Not enough memory","matrix::setName");
    }
    else strcpy(Name,N);
  }
}
