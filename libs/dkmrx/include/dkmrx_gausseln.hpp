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
namespace dkmrx {

	template <class RealNumberA, class RealNumberB>
	size_t gauss_elimination(int A_Columns, int B_Columns, RealNumberA* A_Values, RealNumberB* B_Values)
	{
		RealNumberA* TopPtrA = A_Values + A_Columns * A_Columns;
		int iiStepA = A_Columns + 1;
		int i1iStepA = A_Columns;
		RealNumberB* iPtrB = B_Values;
		RealNumberA* iiPtrA = A_Values;
		RealNumberA* iiStartA = A_Values;
		RealNumberB* jPtrX;
		RealNumberB* i1PtrB;
		RealNumberA* i1iPtrA;
		RealNumberB* ijPtrB = B_Values;
		RealNumberB* i1jPtrB;
		RealNumberA* ijPtrA;
		RealNumberA* i1jPtrA;
		RealNumberB* ijTopB;
		RealNumberA* ijTopA = A_Values + A_Columns;
		int   iStepB = B_Columns;
		size_t   iNumSolutions = 1;
		double  koeff;

		if (B_Columns == 1)
		{
			while (iiPtrA < TopPtrA)
			{
				if (*iiPtrA == 0.0)
					for (i1iPtrA = iiPtrA + i1iStepA, i1jPtrB = ijPtrB + 1;
						i1iPtrA < TopPtrA;
						i1iPtrA += i1iStepA, i1jPtrB++)
						if (*i1iPtrA != 0.0)
						{
							ijPtrA = iiPtrA;
							i1jPtrA = i1iPtrA;
							while (ijPtrA < ijTopA) *ijPtrA++ += *i1jPtrA++;
							*ijPtrB += *i1jPtrB;
							break;
						}
				if (*iiPtrA != 0)
					for (i1iPtrA = iiPtrA + i1iStepA, i1jPtrB = ijPtrB + 1;
						i1iPtrA < TopPtrA;
						i1iPtrA += i1iStepA, i1jPtrB++)
						if (*i1iPtrA != 0.0)
						{
							ijPtrA = iiPtrA;
							i1jPtrA = i1iPtrA;
							koeff = *i1jPtrA++ / *ijPtrA++;
							while (ijPtrA < ijTopA) *i1jPtrA++ -= (RealNumberA)(koeff * *ijPtrA++);
							*i1jPtrB -= (RealNumberB)(koeff * *ijPtrB);
						}
				iiPtrA += iiStepA;
				ijPtrB++;
				ijTopA += i1iStepA;
			}
			for (iiPtrA = TopPtrA - 1, i1iPtrA = TopPtrA, ijPtrB = B_Values + A_Columns - 1;
				iiPtrA >= iiStartA;
				iiPtrA -= iiStepA, --ijPtrB, i1iPtrA -= i1iStepA)
			{
				for (ijPtrA = iiPtrA + 1, jPtrX = ijPtrB + 1; ijPtrA < i1iPtrA; ijPtrA++, jPtrX++)
					*ijPtrB -= (RealNumberB)(*ijPtrA * *jPtrX); //(*x)[j][0];
				if (*iiPtrA == 0.0)
				{
					if (*ijPtrB != 0.0) return 0;
					else
					{
						iNumSolutions++;
						*ijPtrB = (RealNumberB)0.0;
					}
				}
				else *ijPtrB /= (RealNumberB)*iiPtrA;
			}
		}
		else
		{
			while (iiPtrA < TopPtrA)
			{
				if (*iiPtrA == 0.0)
					for (i1iPtrA = iiPtrA + i1iStepA, i1PtrB = iPtrB + iStepB;
						i1iPtrA < TopPtrA;
						i1iPtrA += i1iStepA, i1PtrB += iStepB)
						if (*i1iPtrA != 0.0)
						{
							ijPtrA = iiPtrA; i1jPtrA = i1iPtrA;
							ijPtrB = iPtrB; i1jPtrB = i1PtrB; ijTopB = iPtrB + iStepB;
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
					for (i1iPtrA = iiPtrA + i1iStepA, i1PtrB = iPtrB + iStepB;
						i1iPtrA < TopPtrA;
						i1iPtrA += i1iStepA, i1PtrB += iStepB)
						if (*i1iPtrA != 0.0)
						{
							ijPtrA = iiPtrA; i1jPtrA = i1iPtrA;
							koeff = *i1jPtrA++ / *ijPtrA++;
							ijPtrB = iPtrB; i1jPtrB = i1PtrB; ijTopB = iPtrB + iStepB;
							if ((ijTopA - ijPtrA) <= (ijTopB - ijPtrB))
							{
								while (ijPtrA < ijTopA)
								{
									*i1jPtrA++ -= (RealNumberA)(koeff * *ijPtrA++);
									*i1jPtrB++ -= (RealNumberB)(koeff * *ijPtrB++);
								}
								while (ijPtrB < ijTopB) *i1jPtrB++ -= (RealNumberB)(koeff * *ijPtrB++);
							}
							else
							{
								while (ijPtrB < ijTopB)
								{
									*i1jPtrA++ -= (RealNumberA)(koeff * *ijPtrA++);
									*i1jPtrB++ -= (RealNumberB)(koeff * *ijPtrB++);
								}
								while (ijPtrA < ijTopA) *i1jPtrA++ -= (RealNumberA)(koeff * *ijPtrA++);
							}
						}
				iiPtrA += iiStepA;
				iPtrB += iStepB;
				ijTopA += i1iStepA;
			}
			for (iiPtrA = TopPtrA - 1, i1iPtrA = TopPtrA,
				iPtrB = B_Values + (A_Columns - 1) * B_Columns;
				iiPtrA >= iiStartA;
				iiPtrA -= iiStepA, iPtrB -= iStepB, i1iPtrA -= i1iStepA)
			{
				for (ijPtrB = iPtrB, ijTopB = iPtrB + B_Columns;
					ijPtrB < ijTopB;
					ijPtrB++
					)
				{
					for (ijPtrA = iiPtrA + 1, jPtrX = ijPtrB + iStepB; ijPtrA < i1iPtrA; ijPtrA++, jPtrX += iStepB)
						*ijPtrB -= (RealNumberB)(*ijPtrA * *jPtrX); //(*x)[j][0];       
					if (*iiPtrA != 0.0) *ijPtrB /= (RealNumberB)*iiPtrA;
					else
						if (*ijPtrB != 0.0) return 0;
						else
						{
							iNumSolutions++;
							*ijPtrB = (RealNumberB)0.0;
						}
				}
			}
		}
		return iNumSolutions;
	}

} // namespace dkmrx
#endif // libs_dkmrx_gausseln_hpp
