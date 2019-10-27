/*
  General purpose matrix library, Version 1.0
  Copyright (c) 1994 David Krikheli
 
  Permission to use, copy, modify, distribute, and sell this software and 
  its documentation for any purpose is hereby granted without fee, provided
  that (i) the above copyright notice and this permission notice appear in
  all copies of the software and related documentation, and (ii) the name of
  David Krikheli may not be used in any advertising or publicity relating to
  the software without the specific, prior written permission of David Krikheli.
  
  THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
  EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
  WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 
  IN NO EVENT SHALL David Krikheli BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
  INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
  RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF THE
  POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT OF OR 
  IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef geomKit_h
#define geomKit_h

#include "dkmrx_matrix.hpp"

class gPoint: public matrix
{
public:

gPoint(double x=0.0, double y=0.0, double z=0.0, double w=1.0);
gPoint(gPoint&);

double distance(gPoint&);
inline double x(void)     { return (*this)[0][0]/(*this)[0][3]; }
inline void   x(double X) { (*this)[0][0] = (real)((*this)[0][3] * X); }
inline double y(void)     { return (*this)[0][1]/(*this)[0][3]; }
inline void   y(double Y) { (*this)[0][1] = (real)((*this)[0][3] * Y); }
inline double z(void)     { return (*this)[0][2]/(*this)[0][3]; }
inline void   z(double Z) { (*this)[0][2] = (real)((*this)[0][3] * Z); }
inline double w(void)     { return (*this)[0][3]; }
void   w(double);
};

class gLine: public matrix
{
public:

gLine(gPoint& point1, gPoint& point2, double par1=0.0, double par2=1.0);
gLine(void);
gLine(gLine&);

gPoint& point(double par);
};

class geomKit: public matrix
{   
public:
geomKit();
geomKit(geomKit&);
void rotate(double through,matrix& about, matrix& putFrom );
void rotateX(double through);
void rotateY(double through);
void rotateZ(double through);
void translate(matrix& to, matrix& putFrom );
void translate(double x,double y,double z, matrix& putFrom );
void scale(double Kx,double Ky,double Kz, matrix& about );
void reset(void);

static geomKit&	rotation	(double through,matrix& about, matrix& putFrom );
static geomKit&	rotationX	(double through);
static geomKit&	rotationY	(double through);
static geomKit&	rotationZ	(double through);
static geomKit&	translation	(matrix& to, matrix& putFrom );
static geomKit&	translation	(double x,double y,double z, matrix& putFrom );
static geomKit&	scaling		(double Kx,double Ky,double Kz, matrix& about );
};

#endif //geomKit_h
