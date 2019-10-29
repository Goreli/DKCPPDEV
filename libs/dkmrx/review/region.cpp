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

#include "matrix.hpp"
#include "merror.hpp"

matrix& matrix::extract
(int FirstRow, int FirstCol, int RowsTotal, int ColsTotal)
{
	mError::set();
	if( FirstRow < 0 || FirstRow >= Rows )
	{
		mError::set( MERR_WRONG_ARGUMENT, 1 );
		mError::message("Argument FirstRow out of range","matrix::extract");
		if ( Status==TEMPORARY ) delete this;
		matrix *mx=new matrix;
		mx->Status=TEMPORARY;
		return *mx;

	}
	if( FirstCol < 0 || FirstCol >= Columns )
	{
		mError::set( MERR_WRONG_ARGUMENT, 2 );
		mError::message("Argument FirstCol out of range","matrix::extract");
		if ( Status==TEMPORARY ) delete this;
		matrix *mx=new matrix;
		mx->Status=TEMPORARY;
		return *mx;

	}
	if( RowsTotal < 0 || FirstRow + RowsTotal > Rows )
	{
		mError::set( MERR_WRONG_ARGUMENT, 3 );
		mError::message("Argument RowsTotal out of range","matrix::extract");
		if ( Status==TEMPORARY ) delete this;
		matrix *mx=new matrix;
		mx->Status=TEMPORARY;
		return *mx;

	}
	if( ColsTotal < 0 || FirstCol + ColsTotal > Columns )
	{
		mError::set( MERR_WRONG_ARGUMENT, 4 );
		mError::message("Argument LastCol out of range","matrix::extract");
		if ( Status==TEMPORARY ) delete this;
		matrix *mx=new matrix;
		mx->Status=TEMPORARY;
		return *mx;

	}

matrix *mx = new matrix( RowsTotal, ColsTotal );
	if( mx->Values == NULL )
	{
		mError::set( MERR_INSUFFICIENT_MEMORY );
		mError::message("Not enough memory","matrix::extract");
		if ( Status==TEMPORARY ) delete this;
		mx->Status=TEMPORARY;
		return *mx;
	}

register real *ThisElem;
register real *NemElem = mx->Values;
register real *LastInRow;
register real *FirstInRow;
register real *TopRow;

	FirstInRow = (*this)[FirstRow] + FirstCol;
	LastInRow  = FirstInRow + ColsTotal - 1;
	TopRow     = (*this)[FirstRow + RowsTotal - 1]  + FirstCol;

	while( FirstInRow <= TopRow )
	{
		ThisElem = FirstInRow;
		while( ThisElem <= LastInRow )
			*NemElem++ = *ThisElem++;
		FirstInRow += Columns;
		LastInRow  += Columns;
	}

	if ( Status==TEMPORARY ) delete this;
	mx->Status=TEMPORARY;
	return *mx;
}

void matrix::paste_and_add
	(int FirstToRow, int FirstToCol,
	matrix& From,
	int FirstFromRow, int FirstFromCol,
	int RowsTotal, int ColsTotal)
{
	mError::set();
	if( Status == TEMPORARY )
	{
		mError::set( MERR_WRONG_THIS_OBJECT );
		mError::message("Can not apply to a temporary object","matrix::paste_and_add");
		if ( From.Status==TEMPORARY ) delete &From;
		delete this;
		return;
	}
	if( From.Values == NULL )
	{
		mError::set( MERR_WRONG_ARGUMENT, 3 );
		mError::message("Argument From should not be an empty matrix","matrix::paste_and_add");
		if ( From.Status==TEMPORARY ) delete &From;
		return;
	}
	if( FirstToRow < 0 || FirstToRow >= Rows )
	{
		mError::set( MERR_WRONG_ARGUMENT, 1 );
		mError::message("Argument FirstToRow out of range","matrix::paste_and_add");
		if ( From.Status==TEMPORARY ) delete &From;
		return;
	}
	if( FirstToCol < 0 || FirstToCol >= Columns )
	{
		mError::set( MERR_WRONG_ARGUMENT, 2 );
		mError::message("Argument FirstToCol out of range","matrix::paste_and_add");
		if ( From.Status==TEMPORARY ) delete &From;
		return;
	}
	if( FirstFromRow < 0 || FirstFromRow >= From.Rows )
	{
		mError::set( MERR_WRONG_ARGUMENT, 4 );
		mError::message("Argument FirstFromRow out of range","matrix::paste_and_add");
		if ( From.Status==TEMPORARY ) delete &From;
		return;
	}
	if( FirstFromCol < 0 || FirstFromCol >= From.Columns )
	{
		mError::set( MERR_WRONG_ARGUMENT, 5 );
		mError::message("Argument FirstFromCol out of range","matrix::paste_and_add");
		if ( From.Status==TEMPORARY ) delete &From;
		return;
	}
	if(
		RowsTotal < 0
			||
		FirstToRow + RowsTotal > Rows
			||
		FirstFromRow + RowsTotal > From.Rows
	  )
	{
		mError::set( MERR_WRONG_ARGUMENT, 6 );
		mError::message("Argument RowsTotal out of range","matrix::paste_and_add");
		if ( From.Status==TEMPORARY ) delete &From;
		return;
	}
	if(
		ColsTotal < 0
			||
		FirstToCol + ColsTotal > Columns
			||
		FirstFromCol + ColsTotal > From.Columns
	  )
	{
		mError::set( MERR_WRONG_ARGUMENT, 7 );
		mError::message("Argument LastFromCol out of range","matrix::paste_and_add");
		if ( From.Status==TEMPORARY ) delete &From;
		return;
	}

register real *FromElem;
register real *ThisElem;
register real *LastInRow;
register real *FirstInRow;
register real *TopRow;

	FirstInRow= From[FirstFromRow]  + FirstFromCol;
	LastInRow = FirstInRow + ColsTotal - 1;
	TopRow    = From[FirstFromRow + RowsTotal - 1]   + FirstFromCol;
	ThisElem  = (*this)[FirstToRow] + FirstToCol;

	while( FirstInRow <= TopRow )
	{
		FromElem = FirstInRow;
		while( FromElem <= LastInRow )
			*ThisElem++ += *FromElem++;
		FirstInRow += From.Columns;
		LastInRow  += From.Columns;
		ThisElem   += Columns - ColsTotal;
	}

	if ( From.Status==TEMPORARY ) delete &From;
}
