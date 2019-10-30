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

#include <iostream>
#include <cstring>
#include <cctype>
#include "dkmrx_matrix.hpp"
#include "dkmrx_error.hpp"

using namespace dkmrx;

char* str_lwr(char*);

std::ostream& dkmrx::operator<<(std::ostream& out,matrix& mx)
{
std::streamsize tempWidth     = out.width();
std::streamsize tempPrecision = out.precision();
	if( tempWidth == 0 )
	{
		if( tempPrecision == 0 )
			tempPrecision = 6;
		tempWidth = 2*tempPrecision;
	}

    out<<"Matrix: "; 
    if ( mx.Name != nullptr) out<<mx.Name;
    out<<"\n";
    out<<"rows "<<mx.Rows<<"; columns "<<mx.Columns;
    out<<"; width "<<tempWidth<<"; precision "<<tempPrecision<<"\n";
    
	if ( mx.Values != nullptr)
	for ( int i=0; i<mx.Rows; i++ )
	{
	  for ( int j=0; j<mx.Columns; j++ )
	  {
	    out.width(tempWidth);
	    out.precision(tempPrecision);
	    out<<mx[i][j]<<" "; //Actual width may be more than the specified one.
	  }                     //The blank is a guaranteed delimiter to reading.
	  out<<"\n";
	}
	out<<"\n";

	if ( mx.Status== STATUS::TEMPORARY ) delete &mx;
	return out;
}

std::istream& dkmrx::operator>>(std::istream& in,matrix& mx)
{    
    mError::set();
    if ( mx.Status== STATUS::TEMPORARY )
    {
      delete &mx;
      return in;                                            
    }  
    if ( mx.Values != nullptr)
	mx.empty();
// READ HEADER             
int   chcount = 81; // both header strings are assumed to be less than 80 ch long
char* tempptr;     
	char* str    = new char[chcount];
	if ( str == nullptr)
	{
		mError::set( MERR_INSUFFICIENT_MEMORY );
		mError::message("Not enough memory","matrix::operator >>");
      		return in;
	}
// read and parse the first header row
    do
    {
      in.getline(str,chcount); 
      tempptr = std::strstr(str_lwr(str),"matrix:");
    } 
    while( !tempptr && !in.eof() );
    if( in.eof() )
    {
      delete [] str;
      mError::set( MERR_HEADER_READ, 1 );
      mError::message("can not find first header row","matrix::operator >>");
      return in;
    }                                                       
    tempptr += std::strlen("matrix:");
    while( *tempptr == ' ' )
	tempptr++;
	if(std::strlen(tempptr) != 0 )
		mx.name( tempptr );
// read and and parse the second header row
	in.getline(str,chcount);
    if( in.eof() )
    {
      delete [] str;
      mx.empty();
      mError::set( MERR_HEADER_READ, 2 );
      mError::message("can not find second header row","matrix::operator >>");
      return in;
    }
int flag = 0;
	tempptr = std::strstr(str_lwr(str),"rows");
	if( tempptr != nullptr)
	{
	  tempptr += std::strlen("rows")+1;
	  mx.Rows = std::atoi(tempptr);
	} 
	else flag++;
	tempptr = std::strstr(str_lwr(str),"columns");
	if( tempptr != nullptr)
	{
	  tempptr += std::strlen("columns")+1;
	  mx.Columns = std::atoi(tempptr);
	} 
	else flag++;
int wid=0;
	tempptr = std::strstr(str_lwr(str),"width");
	if( tempptr != nullptr)
	{
	  tempptr += std::strlen("width")+1;
	  wid = std::atoi(tempptr);
	} 
	else flag++;
	if( 
		mx.Rows    == 0  ||
		mx.Columns == 0  ||
		wid        == 0  ||
		flag       != 0
	  )
	{
      delete [] str;
      mx.empty();
      mError::set( MERR_HEADER_READ, 2, 1 );
      mError::message("invalid second header row","matrix::operator >>");
      return in;
	}
// READ VALUES
chcount = mx.Columns * wid * 2;
delete [] str;
	str    = new char[chcount];
	if ( str == nullptr)
	{
	  mx.empty();
	  mError::set( MERR_INSUFFICIENT_MEMORY );
	  mError::message("Not enough memory","matrix::operator >>");
	  return in;
	}
	mx.Values = new real [ mx.Rows * mx.Columns ];
	if ( mx.Values == nullptr)
	{
	  delete [] str;
	  mx.empty();
	  mError::set( MERR_INSUFFICIENT_MEMORY );
	  mError::message("Not enough memory","matrix::operator >>");
	  return in;
	}         
	for(int row=0; row<mx.Rows; row++)
	{   
		in.getline(str,chcount);
		if(in.eof())
		{
		  delete [] str;
		  mx.empty();
		  mError::set( MERR_UNEXPECTED_EOF );
    		  mError::message("Unexpected end of file","matrix::operator >>");
		  return in;
		} 
		tempptr = std::strtok(str," \t,;");
		for(int col=0; col<mx.Columns; col++)
		{
		  mx[row][col] = (real)std::atof(tempptr);
		  tempptr = std::strtok(nullptr," \t,;");
		}
	}
//Done!
	delete [] str;
	return in;
}                   

char* str_lwr(char* str)
{
/*
char* c=str;

	while( *c != '\000' )
	{
		if( isupper(*c) ) *c = (char) tolower(*c);
		c++;
	}
	return str;
*/
char* c = str;
	while (*c != '\000')
		*c++ = (char)std::tolower(*c);
	return str;
}

