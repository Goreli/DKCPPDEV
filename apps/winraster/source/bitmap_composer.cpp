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

#include "bitmap_composer.hpp"
#include "projected_point.hpp"

BitmapComposer::BitmapComposer()
   : bgB_{ 0 }, bgG_{ 0 }, bgR_{ 0 }, 
   iProjectionHeight_{ 0 }, iProjectionWidth_{ 0 }, pProjectionBuffer_{ nullptr },
   iLeftMargin_{ 0 }, iBottomMargin_{ 0 }, numBytesInRow_{ 0 }, pBitmapBuffer_{ nullptr }
{
}
BitmapComposer::~BitmapComposer()
{
}
void BitmapComposer::init(unsigned char bgB, unsigned char bgG, unsigned char bgR)
{
   bgB_ = bgB;
   bgG_ = bgG;
   bgR_ = bgR;
}
void BitmapComposer::setupProjection(size_t iProjectionHeight, size_t iProjectionWidth, ProjectedPoint* pProjectionBuffer)
{
   iProjectionHeight_ = iProjectionHeight;
   iProjectionWidth_ = iProjectionWidth;
   pProjectionBuffer_ = pProjectionBuffer;
}
void BitmapComposer::setupBitmap(size_t iLeftMargin, size_t iBottomMargin, size_t numBytesInRow, unsigned char* pBitmapBuffer)
{
   iLeftMargin_ = iLeftMargin;
   iBottomMargin_ = iBottomMargin;
   numBytesInRow_ = numBytesInRow;
   pBitmapBuffer_ = pBitmapBuffer;
}
void BitmapComposer::operator()(size_t inxBegin, size_t inxEnd)
{
   for (size_t row = inxBegin; row < inxEnd; row++)
   {
      ProjectedPoint* pProjPoint = pProjectionBuffer_ + row * iProjectionWidth_;
      unsigned char* pPixel = pBitmapBuffer_ + (row + iBottomMargin_) * numBytesInRow_ + iLeftMargin_ * 3;

      // These two variables are meant to help getting rid of the ugly
      // black mesh periodically occuring in the middle of the image and
      // supposidly caused by the rounding error.
      bool bThereWasNonBlank{ false };
      bool bHereIsAnotherBlank{ false };

      for (int col = 0; col < iProjectionWidth_; col++)
      {
         if (pProjPoint->counter == 0)
         {
            if (bThereWasNonBlank)
               bHereIsAnotherBlank = true;

            *pPixel++ = bgB_;
            *pPixel++ = bgG_;
            *pPixel++ = bgR_;
         }
         else
         {
            *pPixel++ = static_cast<unsigned char>(pProjPoint->b / pProjPoint->counter);
            *pPixel++ = static_cast<unsigned char>(pProjPoint->g / pProjPoint->counter);
            *pPixel++ = static_cast<unsigned char>(pProjPoint->r / pProjPoint->counter);

            // Check if there was a rounding gap and interpolate its colors if there was one.
            if (bHereIsAnotherBlank)
            {
               // Non-blank pixel after a blank one even though there was
               // already a non-blank pixel before the last blank one...
               // Let's fill the gap with interpolated color.
               unsigned char* pBlankPixel = pPixel - 6;

               // Interpolate blue.
               *pBlankPixel = pBlankPixel[-3] / 2 + pBlankPixel[3] / 2;
               pBlankPixel++;
               // Interpolate green.
               *pBlankPixel = pBlankPixel[-3] / 2 + pBlankPixel[3] / 2;
               pBlankPixel++;
               // Interpolate red.
               *pBlankPixel = pBlankPixel[-3] / 2 + pBlankPixel[3] / 2;
               pBlankPixel++;

               bHereIsAnotherBlank = false;
            }
            bThereWasNonBlank = true;
         }
         pProjPoint++;
      }
      // Possibly we have a padding at the end of each row.
      // Fill it with zeros. This is safe because elements
      // pointed to by pPixel are smaller than those pointed
      // to by pProjPoint. Therefore pPixel has remained well
      // below the upper boundary of allocated space.
      *pPixel = '\0';
      *(pPixel + 1) = '\0';
      *(pPixel + 2) = '\0';
   }
}
size_t BitmapComposer::size()
{
   return iProjectionHeight_;
}
