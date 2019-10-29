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

#ifndef winbmpinfoheader_hpp
#define winbmpinfoheader_hpp

#include <fstream>
#include <Windows.h>
#include "dkmrx_matrix.hpp"

template<typename T>
std::istream& binary_read(std::istream& inStream, T& value) {
	return inStream.read(reinterpret_cast<char*>(&value), sizeof(T));
}

class WinBMPInfoHeader {
private:
	BITMAPINFOHEADER	bmpInfoHeader_{ 0 };

	unsigned long getRGB_(int indexInRow, char* bitsOfRow, RGBQUAD* colorTable);
	size_t getColorTableSize_(void);

public:
	int readInfoHeader(std::istream& inStream);
	void wbmp2mrx(long lOffset, std::istream& inStream, dkmrx::matrix& mrx);
	bool isFormatSupported(void);
	void printProperties(int sizeOfInfoHeader);
	LONG getHeight();
	LONG getWidth();

	static bool readFileHeader(BITMAPFILEHEADER& fileHeader, std::istream& inStream);
};

#endif // winbmpinfoheader_hpp
