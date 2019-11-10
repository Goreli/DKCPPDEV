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

#ifndef libs_dkmrx_gausseln_hpp
#define libs_dkmrx_gausseln_hpp 

#include <type_traits>

namespace dkmrx {

/*
	It's the callers responsibility to make sure that matrix A is square.
	Parameters:
		A_Columns - the number of columns in matrix A. The number of rows is expected
					to equal A_Columns;
		B_Columns - the number of columns in matrix B. The number of rows is expected
					to equal A_Columns;
		A_Values -	a pointer to the data of matrix A;
		B_Values -	a pointer to the data of matrix B. Note that the type of matrix B
					data can be declared different to the ty
	Return value:
		The function returs:
			- 0 if the system of linear equations doesn't have a solution;
			- 1 if it has only one solution;
			- >1 if it has many solutions.
*/
	template <class RealTypeA, class RealTypeB>
	size_t gauss_elimination(RealTypeA* A_Values, const size_t A_Columns, RealTypeB* B_Values, const size_t B_Columns)
	{
		RealTypeA* TopPtrA = A_Values + A_Columns * A_Columns;
		size_t iiStepA = A_Columns + 1;
		RealTypeB* iPtrB = B_Values;
		RealTypeA* iiPtrA = A_Values;
		RealTypeB* jPtrX;
		RealTypeB* i1PtrB;
		RealTypeA* i1iPtrA;
		RealTypeB* ijPtrB = B_Values;
		RealTypeB* i1jPtrB;
		RealTypeA* ijPtrA;
		RealTypeA* i1jPtrA;
		RealTypeB* ijTopB;
		RealTypeA* ijTopA = A_Values + A_Columns;
		size_t   iNumSolutions = 1;
		// Use the most precise of the two types:
		typedef std::conditional<sizeof(RealTypeA) <= sizeof(double), double, RealTypeA>::type koeffType;
		koeffType koeff;

		if (B_Columns == 1)
		{
			while (iiPtrA < TopPtrA)
			{
				if (*iiPtrA == 0.0)
					for (i1iPtrA = iiPtrA + A_Columns, i1jPtrB = ijPtrB + 1;
						i1iPtrA < TopPtrA;
						i1iPtrA += A_Columns, i1jPtrB++)
						if (*i1iPtrA != 0.0)
						{
							ijPtrA = iiPtrA;
							i1jPtrA = i1iPtrA;
							while (ijPtrA < ijTopA) *ijPtrA++ += *i1jPtrA++;
							*ijPtrB += *i1jPtrB;
							break;
						}
				if (*iiPtrA != 0)
					for (i1iPtrA = iiPtrA + A_Columns, i1jPtrB = ijPtrB + 1;
						i1iPtrA < TopPtrA;
						i1iPtrA += A_Columns, i1jPtrB++)
						if (*i1iPtrA != 0.0)
						{
							ijPtrA = iiPtrA;
							i1jPtrA = i1iPtrA;
							koeff = *i1jPtrA++ / *ijPtrA++;
							while (ijPtrA < ijTopA) *i1jPtrA++ -= (RealTypeA)(koeff * *ijPtrA++);
							*i1jPtrB -= (RealTypeB)(koeff * *ijPtrB);
						}
				iiPtrA += iiStepA;
				ijPtrB++;
				ijTopA += A_Columns;
			}
			for (iiPtrA = TopPtrA - 1, i1iPtrA = TopPtrA, ijPtrB = B_Values + A_Columns - 1;
				iiPtrA >= A_Values;
				iiPtrA -= iiStepA, --ijPtrB, i1iPtrA -= A_Columns)
			{
				for (ijPtrA = iiPtrA + 1, jPtrX = ijPtrB + 1; ijPtrA < i1iPtrA; ijPtrA++, jPtrX++)
					*ijPtrB -= (RealTypeB)(*ijPtrA * *jPtrX); //(*x)[j][0];
				if (*iiPtrA == 0.0)
				{
					if (*ijPtrB != 0.0) return 0;
					else
					{
						iNumSolutions++;
						*ijPtrB = (RealTypeB)0.0;
					}
				}
				else *ijPtrB /= (RealTypeB)*iiPtrA;
			}
		}
		else
		{
			while (iiPtrA < TopPtrA)
			{
				if (*iiPtrA == 0.0)
					for (i1iPtrA = iiPtrA + A_Columns, i1PtrB = iPtrB + B_Columns;
						i1iPtrA < TopPtrA;
						i1iPtrA += A_Columns, i1PtrB += B_Columns)
						if (*i1iPtrA != 0.0)
						{
							ijPtrA = iiPtrA; i1jPtrA = i1iPtrA;
							ijPtrB = iPtrB; i1jPtrB = i1PtrB; ijTopB = iPtrB + B_Columns;
							if ((ijTopA - ijPtrA) <= (ijTopB - ijPtrB))
							{
								while (ijPtrA < ijTopA)
								{
									*ijPtrA++ += *i1jPtrA++;
									*ijPtrB++ += *i1jPtrB++;
								}
								while (ijPtrB < ijTopB) *ijPtrB++ += *i1jPtrB++;
							}
							else
							{
								while (ijPtrB < ijTopB)
								{
									*ijPtrA++ += *i1jPtrA++;
									*ijPtrB++ += *i1jPtrB++;
								}
								while (ijPtrA < ijTopA) *ijPtrA++ += *i1jPtrA++;
							}
							break;
						}
				if (*iiPtrA != 0)
					for (i1iPtrA = iiPtrA + A_Columns, i1PtrB = iPtrB + B_Columns;
						i1iPtrA < TopPtrA;
						i1iPtrA += A_Columns, i1PtrB += B_Columns)
						if (*i1iPtrA != 0.0)
						{
							ijPtrA = iiPtrA; i1jPtrA = i1iPtrA;
							koeff = *i1jPtrA++ / *ijPtrA++;
							ijPtrB = iPtrB; i1jPtrB = i1PtrB; ijTopB = iPtrB + B_Columns;
							if ((ijTopA - ijPtrA) <= (ijTopB - ijPtrB))
							{
								while (ijPtrA < ijTopA)
								{
									*i1jPtrA++ -= (RealTypeA)(koeff * *ijPtrA++);
									*i1jPtrB++ -= (RealTypeB)(koeff * *ijPtrB++);
								}
								while (ijPtrB < ijTopB) *i1jPtrB++ -= (RealTypeB)(koeff * *ijPtrB++);
							}
							else
							{
								while (ijPtrB < ijTopB)
								{
									*i1jPtrA++ -= (RealTypeA)(koeff * *ijPtrA++);
									*i1jPtrB++ -= (RealTypeB)(koeff * *ijPtrB++);
								}
								while (ijPtrA < ijTopA) *i1jPtrA++ -= (RealTypeA)(koeff * *ijPtrA++);
							}
						}
				iiPtrA += iiStepA;
				iPtrB += B_Columns;
				ijTopA += A_Columns;
			}
			for (iiPtrA = TopPtrA - 1, i1iPtrA = TopPtrA,
				iPtrB = B_Values + (A_Columns - 1) * B_Columns;
				iiPtrA >= A_Values;
				iiPtrA -= iiStepA, iPtrB -= B_Columns, i1iPtrA -= A_Columns)
			{
				for (ijPtrB = iPtrB, ijTopB = iPtrB + B_Columns;
					ijPtrB < ijTopB;
					ijPtrB++
					)
				{
					for (ijPtrA = iiPtrA + 1, jPtrX = ijPtrB + B_Columns; ijPtrA < i1iPtrA; ijPtrA++, jPtrX += B_Columns)
						*ijPtrB -= (RealTypeB)(*ijPtrA * *jPtrX); //(*x)[j][0];       
					if (*iiPtrA != 0.0)
						*ijPtrB /= (RealTypeB)*iiPtrA;
					else
						if (*ijPtrB != 0.0)
							return 0;
						else
						{
							iNumSolutions++;
							*ijPtrB = (RealTypeB)0.0;
						}
				}
			}
		}
		return iNumSolutions;
	}	// gauss_elimination

	template <class RealType>
	void make_upper_triangular(RealType* pData, const size_t cols)
	{
		RealType* TopPtrA = pData + cols * cols;
		size_t iiStepA = cols + 1;
		RealType* iiPtrA = pData;
		RealType* i1iPtrA;
		RealType* ijPtrA;
		RealType* i1jPtrA;
		RealType* ijTopA = pData + cols;
		// Use the most precise of the two types:
		typedef std::conditional<sizeof(RealType) <= sizeof(double), double, RealType>::type koeffType;
		koeffType koeff;

		while (iiPtrA < TopPtrA)
		{
			if (*iiPtrA == 0.0)
				for (i1iPtrA = iiPtrA + cols;
					i1iPtrA < TopPtrA;
					i1iPtrA += cols)
					if (*i1iPtrA != 0.0)
					{
						ijPtrA = iiPtrA;
						i1jPtrA = i1iPtrA;
						while (ijPtrA < ijTopA)
							*ijPtrA++ += *i1jPtrA++;
						break;
					}
			if (*iiPtrA != 0)
				for (i1iPtrA = iiPtrA + cols;
					i1iPtrA < TopPtrA;
					i1iPtrA += cols)
					if (*i1iPtrA != 0.0)
					{
						ijPtrA = iiPtrA;
						i1jPtrA = i1iPtrA;
						koeff = *i1jPtrA++ / *ijPtrA++;
						while (ijPtrA < ijTopA)
							*i1jPtrA++ -= (RealType)(koeff * *ijPtrA++);
					}
			iiPtrA += iiStepA;
			ijTopA += cols;
		}
		// Done. Let's now set the lower triangle elements to zero values.
		RealType* pRow = pData + cols;
		for (size_t iRow = 1; iRow < cols; iRow++) {
			for (size_t iCol = 0; iCol < iRow; iCol++)
				pRow[iCol] = RealType(0.0);
			pRow += cols;
		}
	}
} // namespace dkmrx
#endif // libs_dkmrx_gausseln_hpp
