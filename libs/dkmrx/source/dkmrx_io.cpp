/*
MIT License

Copyright(c) 2019 David Krikheli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
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
    //if ( mx.pName_ != nullptr) out<<mx.pName_;
    out<<"\n";
    out<<"rows "<<mx.iRows_<<"; columns "<<mx.iColumns_;
    out<<"; width "<<tempWidth<<"; precision "<<tempPrecision<<"\n";
    
	if ( mx.pValues_ != nullptr)
	for ( int i=0; i<mx.iRows_; i++ )
	{
	  for ( int j=0; j<mx.iColumns_; j++ )
	  {
	    out.width(tempWidth);
	    out.precision(tempPrecision);
	    out<<mx[i][j]<<" "; //Actual width may be more than the specified one.
	  }                     //The blank is a guaranteed delimiter to reading.
	  out<<"\n";
	}
	out<<"\n";

	return out;
}

std::istream& dkmrx::operator>>(std::istream& in,matrix& mx)
{    
    if ( mx.pValues_ != nullptr)
		mx.empty();
// READ HEADER             
int   chcount = 81; // both header strings are assumed to be less than 80 ch long
char* tempptr;     
	char* str    = new char[chcount];
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
	  throw std::logic_error("Cannot find the first header row in dkmrx::operator>>(std::istream&, matrix&)");
      //return in;
    }                                                       
    tempptr += std::strlen("matrix:");
    while( *tempptr == ' ' )
		tempptr++;
	//if(std::strlen(tempptr) != 0 )
		//mx.name( tempptr );
// read and and parse the second header row
	in.getline(str,chcount);
    if( in.eof() )
    {
      delete [] str;
      mx.empty();
	  throw std::logic_error("Cannot find the second header row in dkmrx::operator>>(std::istream&, matrix&)");
      //return in;
    }
int flag = 0;
	tempptr = std::strstr(str_lwr(str),"rows");
	if( tempptr != nullptr)
	{
	  tempptr += std::strlen("rows")+1;
	  mx.iRows_ = std::atoi(tempptr);
	} 
	else flag++;
	tempptr = std::strstr(str_lwr(str),"columns");
	if( tempptr != nullptr)
	{
	  tempptr += std::strlen("columns")+1;
	  mx.iColumns_ = std::atoi(tempptr);
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
		mx.iRows_    == 0  ||
		mx.iColumns_ == 0  ||
		wid        == 0  ||
		flag       != 0
	  )
	{
      delete [] str;
      mx.empty();
	  throw std::logic_error("Invalid second header row in dkmrx::operator>>(std::istream&, matrix&)");
      //return in;
	}
// READ VALUES
chcount = mx.iColumns_ * wid * 2;
delete [] str;
	str    = new char[chcount];
	mx.pValues_ = new real [ mx.iRows_ * mx.iColumns_ ];
	for(int row=0; row<mx.iRows_; row++)
	{   
		in.getline(str,chcount);
		if(in.eof())
		{
		  delete [] str;
		  mx.empty();
		  throw std::logic_error("Unexpected end of file in dkmrx::operator>>(std::istream&, matrix&)");
		  //return in;
		} 
		tempptr = std::strtok(str," \t,;");
		for(int col=0; col<mx.iColumns_; col++)
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
char* c = str;
	while (*c != '\000')
		*c++ = (char)std::tolower(*c);
	return str;
}
