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

#ifndef upside_down_projector_hpp
#define upside_down_projector_hpp

class RasterGeometry;
struct ProjectedPoint;

class UpsideDownProjector {
public:
   UpsideDownProjector() noexcept;
   void init(RasterGeometry* pRasterGeom, size_t iProjectionHeight, size_t iProjectionWidth, ProjectedPoint* pProjectionBuffer) noexcept;
   void operator()(size_t inxThread, size_t iNumThreads, size_t iRasterHeight) noexcept;
   void project();

private:
   void project_(size_t inxBeginRow, size_t inxEndRow) noexcept;

   size_t iProjectionWidth_;
   RasterGeometry* pRasterGeom_;
   ProjectedPoint* pProjectedData_;
};

#endif // upside_down_projector_hpp
