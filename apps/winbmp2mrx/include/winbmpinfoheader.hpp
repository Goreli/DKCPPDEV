// winbmpinfoheader.hpp : This file contains a declaration of the WinBMPFileHeader class.
//

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
	void wbmp2mrx(long lOffset, std::istream& inStream, matrix& mrx);
	bool isFormatSupported(void);
	void printProperties(int sizeOfInfoHeader);
	LONG getHeight();
	LONG getWidth();

	static bool readFileHeader(BITMAPFILEHEADER& fileHeader, std::istream& inStream);
};

#endif // winbmpinfoheader_hpp
