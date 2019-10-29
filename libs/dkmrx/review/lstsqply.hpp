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

#ifndef lstsqply_hpp_included
#define lstsqply_hpp_included

//template <class RealNumberA, class RealNumberB>
//int gauss_elimination(int A_Columns,int B_Columns,RealNumberA* A_Values,RealNumberB* B_Values)

#include <math.h>
#include "Gausseln.hpp"

/* bool calc_least_square_poly_coeffs(double* dataX, double* dataY, int numDataPoints, double* coeffs, int numCoeffs)

	Parameters:
		numDataPoints - the number of data points defined in the buffers above;
		dataX - pointer to a buffer containing the X values;
		dataY - pointer to a buffer containing the respective Y values;
		numCoeffs - the size of the coeffs buffer;
		coeffs - a buffer to be populated with the coefficients;
		miscBuffer - a buffer used for miscellanious purposes.
			The size of the buffer has to be at least "numCoeffs*numCoeffs".

	returns:
		True - if successfull;
		false - otherwise.
*/
template <class RealNumberX, class RealNumberY, class RealNumberC>
bool calc_least_square_poly_coeffs(const int numDataPoints, const RealNumberX* dataX, const RealNumberY* dataY, const int numCoeffs, RealNumberC* coeffs, double* miscBuffer)
{
bool success = true;
double tempSum = 0.0;

	// Calculate the column values.
	for(int inxCoeff=0; inxCoeff<numCoeffs; inxCoeff++)
	{
		tempSum = 0.0;
		for(int inxPoint=0; inxPoint<numDataPoints; inxPoint++)
			tempSum += dataY[inxPoint] * pow(dataX[inxPoint], inxCoeff);
		coeffs[inxCoeff] = (RealNumberC)tempSum;
	}

	// Calculate the first row of the matrix.
	for(int inxCol = 0; inxCol < numCoeffs; inxCol++)
	{
		tempSum = 0.0;
		for(int inxPoint=0; inxPoint<numDataPoints; inxPoint++)
			tempSum += pow(dataX[inxPoint], (double)inxCol);
		miscBuffer[inxCol] = tempSum;
	}

	// Calculate the last column of the matrix.
	for(int inxRow = 1; inxRow < numCoeffs; inxRow++)
	{
		tempSum = 0.0;
		for(int inxPoint=0; inxPoint<numDataPoints; inxPoint++)
			tempSum += pow(dataX[inxPoint], inxRow + (numCoeffs-1));
		miscBuffer[inxRow*numCoeffs + numCoeffs - 1] = tempSum;
	}

	// Populate the rest of the matrix.
	for(inxRow = 1; inxRow < numCoeffs; inxRow++)
		for(inxCol = 0; inxCol < numCoeffs-1; inxCol++)
			miscBuffer[inxRow*numCoeffs + inxCol] = miscBuffer[(inxRow-1)*numCoeffs + inxCol+1];

	success = (0 == gauss_elimination(numCoeffs, 1, miscBuffer, coeffs));
	return success;
}

template <class RealNumberX, class RealNumberY, class RealNumberC>
void calc_least_square_poly_values(const int numDataPoints, const RealNumberX* dataX, RealNumberY* dataY, const int numCoeffs, const RealNumberC* coeffs)
{
double tempResult = 0.0;
	for(int inxDataPoint = 0; inxDataPoint < numDataPoints; inxDataPoint++)
	{
		tempResult = 0.0;
		for(int inxCoeff = 0; inxCoeff < numCoeffs; inxCoeff++)
			tempResult += coeffs[inxCoeff]*pow(dataX[inxDataPoint], inxCoeff);
		dataY[inxDataPoint] = (RealNumberY)tempResult;
	}
}

#endif // lstsqply_hpp_included
