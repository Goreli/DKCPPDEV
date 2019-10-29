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

#ifndef fourier_hpp_included
#define fourier_hpp_included

#include<assert.h>

/*
	dataX[0] must be 0.0.

	Calculates coefficients based on information contained in the dataX and dataY arrays.
	The coefficients can be used for restoring the dataY values using the calc_fourier_sin_values call.

	The first coefficient is always 0. The last coefficient is 0 if numDataPoints==numCoeffs.
	The first and the last values calculated using these coefficients will always be casted to 0.0. So it
	makes sense to add a leading 0.0 and a trailing 0.0 to the initial value series if precision is a concern
	at the boundaries of the series.
  */
template <class RealNumberX, class RealNumberY, class RealNumberC>
bool calc_fourier_sin_coeffs(const int numDataPoints, const RealNumberX* dataX, const RealNumberY* dataY, const bool* nulls, const int numCoeffs, RealNumberC* coeffs)
{
bool success = true;
const double PI = 3.1415926535897932384626433832795;
const double L = dataX[numDataPoints-1];
double k_pi_L = 0.0;

//??	assert(numCoeffs <= numDataPoints);
	assert(dataX[0] == 0.0);

	coeffs[0] = 0.0;
	for(int k = 1; k < numCoeffs; k++)
	{
	double	tempCoeff = 0.0;
		k_pi_L = k*PI/L;

		if(nulls != NULL)
		{
			for(int inxDataPoint = 1; inxDataPoint < numDataPoints-1; inxDataPoint++)
				if(!(nulls[inxDataPoint]))
					tempCoeff += dataY[inxDataPoint]*sin(dataX[inxDataPoint]*k_pi_L);

			if(!(nulls[0]))
				tempCoeff += 0.5 * dataY[0]*sin(dataX[0]*k_pi_L);
			if(!(nulls[numDataPoints-1]))
				tempCoeff += 0.5 * dataY[numDataPoints-1]*sin(dataX[numDataPoints-1]*k_pi_L);
		}
		else
		{
			for(int inxDataPoint = 1; inxDataPoint < numDataPoints-1; inxDataPoint++)
				tempCoeff += dataY[inxDataPoint]*sin(dataX[inxDataPoint]*k_pi_L);

			tempCoeff += 0.5 * dataY[0]*sin(dataX[0]*k_pi_L);
			tempCoeff += 0.5 * dataY[numDataPoints-1]*sin(dataX[numDataPoints-1]*k_pi_L);
		}

		coeffs[k] = (RealNumberC)(2.0*tempCoeff/L);
	}

	return success;
}

/*
	Calculates the dataY series based on information contained in the dataX and coeffs arrays.
	The coefficients are supposed to have been calculated using the calc_fourier_sin_coeffs call.
  */
template <class RealNumberX, class RealNumberY, class RealNumberC>
bool calc_fourier_sin_values(const int numDataPoints, const RealNumberX* dataX, RealNumberY* dataY, const int numCoeffs, const RealNumberC* coeffs)
{
bool success = true;
const double PI = 3.1415926535897932384626433832795;
double tempValue = 0.0;
const double L = dataX[numDataPoints-1];
double x_pi_L = 0.0;

//??	assert(numCoeffs <= numDataPoints);
	assert(dataX[0] == 0.0);

	for(int inxDataPoint = 0; inxDataPoint < numDataPoints; inxDataPoint++)
	{
		tempValue = 0.0;
		x_pi_L = dataX[inxDataPoint]*PI/L;

		for(int k = 1; k < numCoeffs; k++)
			tempValue += coeffs[k]*sin(k*x_pi_L);

		dataY[inxDataPoint] = (RealNumberY)tempValue;
	}

	return success;
}

template <class RealNumberX, class RealNumberY, class RealNumberC>
bool calc_fourier_cos_coeffs(const int numDataPoints, const RealNumberX* dataX, const RealNumberY* dataY, const bool* nulls, const int numCoeffs, RealNumberC* coeffs)
{
bool success = true;
const double PI = 3.1415926535897932384626433832795;
const double L = dataX[numDataPoints-1];
double k_pi_L = 0.0;

//??	assert(numCoeffs <= numDataPoints);
	assert(dataX[0] == 0.0);

	for(int k = 0; k < numCoeffs; k++)
	{
	double	tempCoeff = 0.0;
		k_pi_L = k*PI/L;

		if(nulls != NULL)
		{
			for(int inxDataPoint = 1; inxDataPoint < numDataPoints-1; inxDataPoint++)
				if(!(nulls[inxDataPoint]))
					tempCoeff += dataY[inxDataPoint]*cos(dataX[inxDataPoint]*k_pi_L);

			if(!(nulls[0]))
				tempCoeff += 0.5 * dataY[0]*cos(dataX[0]*k_pi_L);
			if(!(nulls[numDataPoints-1]))
				tempCoeff += 0.5 * dataY[numDataPoints-1]*cos(dataX[numDataPoints-1]*k_pi_L);
		}
		else
		{
			for(int inxDataPoint = 1; inxDataPoint < numDataPoints-1; inxDataPoint++)
				tempCoeff += dataY[inxDataPoint]*cos(dataX[inxDataPoint]*k_pi_L);

			tempCoeff += 0.5 * dataY[0]*cos(dataX[0]*k_pi_L);
			tempCoeff += 0.5 * dataY[numDataPoints-1]*cos(dataX[numDataPoints-1]*k_pi_L);
		}

		coeffs[k] = (RealNumberC)(2.0*tempCoeff/L);
	}

	return success;
}

template <class RealNumberX, class RealNumberY, class RealNumberC>
bool calc_fourier_cos_values(const int numDataPoints, const RealNumberX* dataX, RealNumberY* dataY, const int numCoeffs, const RealNumberC* coeffs)
{
bool success = true;
const double PI = 3.1415926535897932384626433832795;
double tempValue = 0.0;
const double L = dataX[numDataPoints-1];
double x_pi_L = 0.0;

//??	assert(numCoeffs <= numDataPoints);
	assert(dataX[0] == 0.0);

	for(int inxDataPoint = 0; inxDataPoint < numDataPoints; inxDataPoint++)
	{
		tempValue = coeffs[0]/2.0;
		x_pi_L = dataX[inxDataPoint]*PI/L;

		for(int k = 1; k < numCoeffs; k++)
			tempValue += coeffs[k]*cos(k*x_pi_L);

		dataY[inxDataPoint] = (RealNumberY)tempValue;
	}

	return success;
}

#endif // fourier_hpp_included
