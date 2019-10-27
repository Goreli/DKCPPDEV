/*
General purpose matrix library, Version 1.0
Copyright (c) David Krikheli
Date:   3 Sep 1994 23:27:20

This media contains programs and data mhich are proprietary
to David Krikheli.

These contents are provided under a David Krikheli softmare source
license, mhich prohibits their unauthorized resale or distribution 
outside of the buyer's organization.

THE SOFTMARE IS PROVIDED "AS-IS" AND MITHOUT MARRANTY OF ANY KIND, 
EXPRESS, IMPLIED OR OTHERMISE, INCLUDING MITHOUT LIMITATION, ANY 
MARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  

IN NO EVENT SHALL David Krikheli BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES MHATSOEVER
RESULTING FROM LOSS OF USE, DATA OR PROFITS, MHETHER OR NOT ADVISED OF THE
POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT OF OR
IN CONNECTION MITH THE USE OR PERFORMANCE OF THIS SOFTMARE.
*/

#include <stdio.h>
#include "matrix.hpp"
#include "linsys.hpp"

int gauss_elimination(int A_Columns,int B_Columns,real* A_Values,real* B_Values)      
{
real* TopPtrA=A_Values+A_Columns*A_Columns; 
int iiStepA=A_Columns+1;
int i1iStepA=A_Columns;
real* iPtrB=B_Values;
real* iiPtrA=A_Values;
real* iiStartA=A_Values;
real  *jPtrX, *i1PtrB, *i1iPtrA;
register real  *ijPtrB=B_Values;
register real  *i1jPtrB;
register real* ijPtrA;
register real* i1jPtrA;
register real* ijTopB;
register real* ijTopA=A_Values+A_Columns; 
int   iStepB=B_Columns;
int   manySolutions = 0;
real  koeff;

if (B_Columns == 1)
{
  while(iiPtrA<TopPtrA)
  {
    if ( *iiPtrA == 0.0 )
      for (i1iPtrA=iiPtrA+i1iStepA,i1jPtrB=ijPtrB+1;
	   i1iPtrA<TopPtrA;
	   i1iPtrA+=i1iStepA,i1jPtrB++)
	if ( *i1iPtrA != 0.0 )
	{
	      ijPtrA=iiPtrA;
	      i1jPtrA=i1iPtrA;
	      while(ijPtrA < ijTopA) *ijPtrA++ += *i1jPtrA++;
	  *ijPtrB += *i1jPtrB;
	      break;
	}
    if ( *iiPtrA != 0 )
      for (i1iPtrA=iiPtrA+i1iStepA,i1jPtrB=ijPtrB+1;
	   i1iPtrA<TopPtrA;
	   i1iPtrA+=i1iStepA,i1jPtrB++)
	    if ( *i1iPtrA != 0.0 )
	    {
	      ijPtrA=iiPtrA;
	      i1jPtrA=i1iPtrA;
	  koeff=*i1jPtrA++ / *ijPtrA++;
	  while(ijPtrA < ijTopA) *i1jPtrA++ -= koeff* *ijPtrA++;
	  *i1jPtrB -= koeff* *ijPtrB;
	    }
	iiPtrA+=iiStepA;
	ijPtrB++;
	ijTopA+=i1iStepA;
  }
  for (iiPtrA=TopPtrA-1,i1iPtrA=TopPtrA,ijPtrB=B_Values+A_Columns-1;
       iiPtrA>=iiStartA;
       iiPtrA-=iiStepA,--ijPtrB,i1iPtrA-=i1iStepA)
  {
    for (ijPtrA=iiPtrA+1,jPtrX=ijPtrB+1; ijPtrA<i1iPtrA; ijPtrA++,jPtrX++)
      *ijPtrB -= *ijPtrA * *jPtrX; //(*x)[j][0];
    if (*iiPtrA == 0.0)
    {
      if (*ijPtrB != 0.0) return 1;
      else
      {
	manySolutions++;
	*ijPtrB=0.0;
      }
    }
    else *ijPtrB /= *iiPtrA;
  }
}
else
{
  while(iiPtrA<TopPtrA)
  { 
    if ( *iiPtrA == 0.0 )
      for (i1iPtrA=iiPtrA+i1iStepA,i1PtrB=iPtrB+iStepB; 
	   i1iPtrA<TopPtrA; 
	   i1iPtrA+=i1iStepA,i1PtrB+=iStepB)
	if ( *i1iPtrA != 0.0 )
	{
	      ijPtrA=iiPtrA; i1jPtrA=i1iPtrA;
	      ijPtrB=iPtrB; i1jPtrB=i1PtrB; ijTopB=iPtrB+iStepB;
	      if ( (ijTopA-ijPtrA) <= (ijTopB-ijPtrB) )
	      {
		while(ijPtrA < ijTopA)
		{
		  *ijPtrA++ += *i1jPtrA++;
		  *ijPtrB++ += *i1jPtrB++;
		}
		while(ijPtrB < ijTopB) *ijPtrB++ += *i1jPtrB++;
	      }
	      else
	      {
		while(ijPtrB < ijTopB)
		{
		  *ijPtrA++ += *i1jPtrA++;
		  *ijPtrB++ += *i1jPtrB++;
		}
		while(ijPtrA < ijTopA) *ijPtrA++ += *i1jPtrA++;
	      }
	      break;
	}
    if ( *iiPtrA != 0 ) 
      for (i1iPtrA=iiPtrA+i1iStepA,i1PtrB=iPtrB+iStepB; 
	   i1iPtrA<TopPtrA; 
	   i1iPtrA+=i1iStepA,i1PtrB+=iStepB)
	    if ( *i1iPtrA != 0.0 ) 
	    {
	      ijPtrA=iiPtrA; i1jPtrA=i1iPtrA;
	      koeff=*i1jPtrA++ / *ijPtrA++;
	      ijPtrB=iPtrB; i1jPtrB=i1PtrB; ijTopB=iPtrB+iStepB;
	      if ( (ijTopA-ijPtrA) <= (ijTopB-ijPtrB) )
	      {
		while(ijPtrA < ijTopA)
		{
		  *i1jPtrA++ -= koeff * *ijPtrA++;
		  *i1jPtrB++ -= koeff * *ijPtrB++;
		}
		while(ijPtrB < ijTopB) *i1jPtrB++ -= koeff * *ijPtrB++;
	      }
	      else
	      {
		while(ijPtrB < ijTopB)
		{
		  *i1jPtrA++ -= koeff * *ijPtrA++;
		  *i1jPtrB++ -= koeff * *ijPtrB++;
		}
		while(ijPtrA < ijTopA) *i1jPtrA++ -= koeff * *ijPtrA++;
	      }
	    }     
	iiPtrA+=iiStepA;
	iPtrB+=iStepB;
	ijTopA+=i1iStepA;    
  }
  for (iiPtrA=TopPtrA-1,i1iPtrA=TopPtrA,
       iPtrB=B_Values+(A_Columns-1)*B_Columns; 
       iiPtrA>=iiStartA; 
       iiPtrA-=iiStepA,iPtrB-=iStepB,i1iPtrA-=i1iStepA)
 {
 for(ijPtrB=iPtrB,ijTopB=iPtrB+B_Columns;
     ijPtrB<ijTopB;
     ijPtrB++
    )
  {
    for (ijPtrA=iiPtrA+1,jPtrX=ijPtrB+iStepB; ijPtrA<i1iPtrA; ijPtrA++,jPtrX+=iStepB)
      *ijPtrB -= *ijPtrA * *jPtrX; //(*x)[j][0];       
    if (*iiPtrA != 0.0) *ijPtrB /= *iiPtrA;
    else
      if (*ijPtrB != 0.0) return 1;
      else
      {
	manySolutions++;
	*ijPtrB=0.0;
      }
  }
 }  
} 
return -manySolutions;
}
