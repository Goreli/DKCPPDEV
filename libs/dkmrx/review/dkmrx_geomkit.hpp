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
