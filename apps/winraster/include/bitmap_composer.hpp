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

#ifndef bitmap_composer_hpp
#define bitmap_composer_hpp

#include "parallel_processor_base.hpp"

class RasterGeometry;
struct ProjectedPoint;

class BitmapComposer : public dk::ParallelProcessorBase {
public:
   BitmapComposer();
   virtual ~BitmapComposer() override;
   void init(unsigned char bgB, unsigned char bgG, unsigned char bgR);

   void setupProjection(size_t iProjectionHeight, size_t iProjectionWidth, ProjectedPoint* pProjectionBuffer);
   void setupBitmap(size_t iLeftMargin, size_t iBottomMargin, size_t numBytesInRow, unsigned char* pBitmapBuffer);
   virtual void operator()(size_t inxBegin, size_t inxEnd) override;
   virtual size_t size() override;

private:
   unsigned char bgB_;
   unsigned char bgG_;
   unsigned char bgR_;

   size_t iProjectionHeight_;
   size_t iProjectionWidth_;
   ProjectedPoint* pProjectionBuffer_;

   size_t iLeftMargin_;
   size_t iBottomMargin_;
   size_t numBytesInRow_;
   unsigned char* pBitmapBuffer_;
};

#endif // bitmap_composer_hpp
