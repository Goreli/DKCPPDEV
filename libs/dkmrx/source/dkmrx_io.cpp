/*
General purpose matrix library, Version 1.0
Copyright (c) David Krikheli
Date:   3 Sep 1994 23:27:20

This media contains programs and data mhich are proprietary
to David Krikheli.

These contents are provided under a David Krikheli software source
license, which prohibits their unauthorized resale or distribution 
outside of the buyer's organization.

THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  

IN NO EVENT SHALL David Krikheli BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF THE
POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT OF OR
IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "dkmrx_matrix.hpp"
#include "dkmrx_error.hpp"

char* str_lwr(char*);

ostream& operator<<(ostream& out,matrix& mx)
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
    if ( mx.Name != NULL ) out<<mx.Name;
    out<<"\n";
    out<<"rows "<<mx.Rows<<"; columns "<<mx.Columns;
    out<<"; width "<<tempWidth<<"; precision "<<tempPrecision<<"\n";
    
	if ( mx.Values != NULL )
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

istream& operator>>(istream& in,matrix& mx)
{    
    mError::set();
    if ( mx.Status== STATUS::TEMPORARY )
    {
      delete &mx;
      return in;                                            
    }  
    if ( mx.Values != NULL )
	mx.empty();
// READ HEADER             
int   chcount = 81; // both header strings are assumed to be less than 80 ch long
char* tempptr;     
	char* str    = new char[chcount];
	if ( str == NULL )
	{
		mError::set( MERR_INSUFFICIENT_MEMORY );
		mError::message("Not enough memory","matrix::operator >>");
      		return in;
	}
// read and parse the first header row
    do
    {
      in.getline(str,chcount); 
      tempptr = strstr(str_lwr(str),"matrix:");
    } 
    while( !tempptr && !in.eof() );
    if( in.eof() )
    {
      delete [] str;
      mError::set( MERR_HEADER_READ, 1 );
      mError::message("can not find first header row","matrix::operator >>");
      return in;
    }                                                       
    tempptr += strlen("matrix:");
    while( *tempptr == ' ' )
	tempptr++;
	if( strlen(tempptr) != 0 )
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
	tempptr = strstr(str_lwr(str),"rows");
	if( tempptr != NULL )
	{
	  tempptr += strlen("rows")+1;
	  mx.Rows = atoi(tempptr);
	} 
	else flag++;
	tempptr = strstr(str_lwr(str),"columns");
	if( tempptr != NULL )
	{
	  tempptr += strlen("columns")+1;
	  mx.Columns = atoi(tempptr);
	} 
	else flag++;
int wid=0;
	tempptr = strstr(str_lwr(str),"width");
	if( tempptr != NULL )
	{
	  tempptr += strlen("width")+1;
	  wid = atoi(tempptr);
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
	if ( str == NULL )
	{
	  mx.empty();
	  mError::set( MERR_INSUFFICIENT_MEMORY );
	  mError::message("Not enough memory","matrix::operator >>");
	  return in;
	}
	mx.Values = new real [ mx.Rows * mx.Columns ];
	if ( mx.Values == NULL )
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
		tempptr = strtok(str," \t,;");
		for(int col=0; col<mx.Columns; col++)
		{
		  mx[row][col] = (real) atof(tempptr);
		  tempptr = strtok(NULL," \t,;");
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
		*c++ = (char)tolower(*c);
	return str;
}

