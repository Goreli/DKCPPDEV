/*
General purpose matrix library, Version 1.0
Copyright (c) David Krikheli
Date:   3 Sep 1994 23:27:20

This media contains programs and data which are proprietary
to David Krikheli.

These contents are provided under a David Krikheli software source
license, which prohibits their unauthorized resale or distribution 
outside of the buyer's organization.

THE SOFTMARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  

IN NO EVENT SHALL David Krikheli BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF THE
POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT OF OR
IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

  31/May/02 - DK created.
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
