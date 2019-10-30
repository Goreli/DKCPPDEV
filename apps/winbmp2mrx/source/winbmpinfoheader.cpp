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

#include <iostream>
#include "winbmpinfoheader.hpp"
using namespace std;


bool WinBMPInfoHeader::readFileHeader(BITMAPFILEHEADER& fileHeader, istream& inStream)
{
	binary_read(inStream, fileHeader);
	if (inStream)
	{
		if ((fileHeader.bfType & 0xFF) == 'B' && ((fileHeader.bfType & 0xFF00) >> 8) == 'M')
			//return fileHeader.bfOffBits;
			if (fileHeader.bfOffBits)
				return true;
		else
			cerr << "winbmp2mrx: this is not a Windows bitmap file.";
	}
	else
		cerr << "winbmp2mrx: unable to read the file header.";

	return false;
}

int WinBMPInfoHeader::readInfoHeader(istream& inStream)
{
	binary_read(inStream, bmpInfoHeader_.biSize);
	inStream.read(reinterpret_cast<char*>(&(bmpInfoHeader_.biWidth)), bmpInfoHeader_.biSize - sizeof(bmpInfoHeader_.biSize));

	return  bmpInfoHeader_.biSize;
}

LONG WinBMPInfoHeader::getHeight()
{
	return bmpInfoHeader_.biHeight;
}

LONG WinBMPInfoHeader::getWidth()
{
	return bmpInfoHeader_.biWidth;
}

void WinBMPInfoHeader::wbmp2mrx(long lOffset, istream& inStream, dkmrx::matrix& mrx)
{
	size_t colorTableSize = getColorTableSize_();
	unique_ptr<RGBQUAD[]> pColorTable;

	if (colorTableSize)
	{
		pColorTable = make_unique<RGBQUAD[]>(colorTableSize);
		//fileReader.read(pColorTable.get(), sizeof(RGBQUAD), colorTableSize);
		inStream.read(reinterpret_cast<char*>(pColorTable.get()), sizeof(RGBQUAD) * colorTableSize);
	}

	int numOfBytesInRow =
		(int)ceil(bmpInfoHeader_.biWidth * bmpInfoHeader_.biBitCount / 8);
	if (numOfBytesInRow % 4 != 0)
		numOfBytesInRow += 4 - numOfBytesInRow % 4;
	unique_ptr<char[]> pBitmapBits = make_unique<char[]>(numOfBytesInRow);

	// Position the file reader at the start of the image data.
	inStream.seekg(lOffset, inStream.beg);

	// Read the image data and write it into the matrix object.
	for (int row = 0; row < bmpInfoHeader_.biHeight; row++)
	{
		// Read 1 row of bitmap image data.
		inStream.read(reinterpret_cast<char*>(pBitmapBits.get()), numOfBytesInRow);

		// Write image color data into the matrix.
		for (int col = 0; col < bmpInfoHeader_.biWidth; col++)
			mrx[bmpInfoHeader_.biHeight - row - 1][col] =
				getRGB_(col, pBitmapBits.get(), pColorTable.get());
	}
}

bool WinBMPInfoHeader::isFormatSupported()
{
	if (bmpInfoHeader_.biCompression != BI_RGB)
	{
		cout << "winbmp2mrx: unsupported bitmap format different to BI_RGB.";
		return false;
	}
	if (bmpInfoHeader_.biBitCount != 8
		&&
		bmpInfoHeader_.biBitCount != 4
		&&
		bmpInfoHeader_.biBitCount != 24
		)
	{
		cout << "winbmp2mrx: only supports the BI_RGB format with 24, 8 or 4 bits per pixel.";
		return false;
	}
	return true;
}

void WinBMPInfoHeader::printProperties(int sizeOfInfoHeader)
{
	cout << "biSize = " << bmpInfoHeader_.biSize << endl;
	cout << "biWidth = " << bmpInfoHeader_.biWidth << endl;
	cout << "biHeight = " << bmpInfoHeader_.biHeight << endl;
	cout << "biPlanes = " << bmpInfoHeader_.biPlanes << endl;
	cout << "biBitCount = " << bmpInfoHeader_.biBitCount << endl;

	int sizeOfFields =
		sizeof(bmpInfoHeader_.biSize) +
		sizeof(bmpInfoHeader_.biWidth) +
		sizeof(bmpInfoHeader_.biHeight) +
		sizeof(bmpInfoHeader_.biPlanes) +
		sizeof(bmpInfoHeader_.biBitCount);

	if (sizeOfInfoHeader > sizeOfFields)
	{
		cout << "biCompression = ";
		switch (bmpInfoHeader_.biCompression)
		{
		case BI_RGB:
			cout << "BI_RGB";
			break;
		case BI_RLE8:
			cout << "BI_RLE8";
			break;
		case BI_RLE4:
			cout << "BI_RLE4";
			break;
		case BI_BITFIELDS:
			cout << "BI_BITFIELDS";
			break;
		default:
			cout << bmpInfoHeader_.biCompression;
			break;
		}
		cout << endl;
		sizeOfFields += sizeof(bmpInfoHeader_.biCompression);
	}
	if (sizeOfInfoHeader > sizeOfFields)
	{
		cout << "biSizeImage = " << bmpInfoHeader_.biSizeImage << endl;
		sizeOfFields += sizeof(bmpInfoHeader_.biSizeImage);
	}
	if (sizeOfInfoHeader > sizeOfFields)
	{
		cout << "biXPelsPerMeter = " << bmpInfoHeader_.biXPelsPerMeter << endl;
		sizeOfFields += sizeof(bmpInfoHeader_.biXPelsPerMeter);
	}
	if (sizeOfInfoHeader > sizeOfFields)
	{
		cout << "biYPelsPerMeter = " << bmpInfoHeader_.biYPelsPerMeter << endl;
		sizeOfFields += sizeof(bmpInfoHeader_.biYPelsPerMeter);
	}
	if (sizeOfInfoHeader > sizeOfFields)
	{
		cout << "biClrUsed = " << bmpInfoHeader_.biClrUsed << endl;
		sizeOfFields += sizeof(bmpInfoHeader_.biClrUsed);
	}
	if (sizeOfInfoHeader > sizeOfFields)
	{
		cout << "biClrImportant = " << bmpInfoHeader_.biClrImportant << endl;
		sizeOfFields += sizeof(bmpInfoHeader_.biClrImportant);
	}
}

unsigned long WinBMPInfoHeader::getRGB_(int indexToRow, char* bitsOfRow, RGBQUAD* colorTable)
{
	unsigned long indexToColorTable{ 0 };
	RGBQUAD* entry = nullptr;
	unsigned long rgb{ 0 };

	if (bmpInfoHeader_.biCompression != BI_RGB)
		return rgb;

	switch (bmpInfoHeader_.biBitCount)
	{
		case 4:
			{
				int offset = indexToRow / 2;
				if (offset * 2 == indexToRow)
					indexToColorTable = 0xF & bitsOfRow[offset];
				else
					indexToColorTable = (0xF0 & bitsOfRow[offset]) >> 4;
				entry = colorTable + indexToColorTable;
				rgb = entry->rgbRed +
					(entry->rgbGreen << 8) +
					(entry->rgbBlue << 16);
			}
			break;
		case 8:
			{
				indexToColorTable = bitsOfRow[indexToRow];
				entry = colorTable + indexToColorTable;
				rgb = entry->rgbRed +
					(entry->rgbGreen << 8) +
					(entry->rgbBlue << 16);
			}
			break;
		case 24:
			{
				int offset = 3 * indexToRow;
				entry = (RGBQUAD*)(bitsOfRow + offset);
				rgb = entry->rgbRed +
					(entry->rgbGreen << 8) +
					(entry->rgbBlue << 16);
			}
			break;
	}

	return rgb;
}

size_t WinBMPInfoHeader::getColorTableSize_()
{
	size_t colorTableSize{ 0 };
	if (bmpInfoHeader_.biCompression != BI_RGB)
		return colorTableSize;

	colorTableSize = bmpInfoHeader_.biClrUsed;
	switch (bmpInfoHeader_.biBitCount)
	{
		case 4:
			if (colorTableSize == 0)
				colorTableSize = 16;
			break;
		case 8:
			if (colorTableSize == 0)
				colorTableSize = 256;
			break;
		case 24:
			colorTableSize = 0;
			break;
	}

	return colorTableSize;
}

