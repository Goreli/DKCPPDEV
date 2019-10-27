// winbmp2mrx.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "winbmpinfoheader.hpp"
#include "dkmrx_matrix.hpp"

using namespace std;

// Forward declarations of functions defined in this module.
void printCopyright(void);
void printUsage(void);
void writeMatrix(matrix& mrx, const char* strOutFileName);

int main(int iArgC, char** lplpszArgV) 
{
	if (iArgC < 2 || iArgC > 3) {
		printCopyright();
		printUsage();
		return 0;
	}

	char* strInFileName = *(lplpszArgV + 1);
	ifstream inStream(strInFileName, ios::binary);
	if (!inStream) {
		cerr << "winbmp2mrx : unable to open file " << strInFileName << endl;
		return 1;
	}

	BITMAPFILEHEADER fileHeader;
	if (!WinBMPInfoHeader::readFileHeader(fileHeader, inStream))
		return 2;
		
	WinBMPInfoHeader winBmpInfoHeader;
	// This doesn't implement a case where the size of the actual structure
	// is greater than sizeof( BITMAPINFOHEADER ).
	// This is because we should have allocated enough space for reading.
	// Refer Petzold, page 608, last sentence in the third paragraph.
	int sizeOfInfoHeader = winBmpInfoHeader.readInfoHeader(inStream);
	printCopyright();
	winBmpInfoHeader.printProperties(sizeOfInfoHeader);

	// If the output file name is not specified on the command line then exit.
	if (iArgC != 3)
		return 0;

	// Exit with an error code if we cannot read the format of the image data.
	if (!winBmpInfoHeader.isFormatSupported())
		return 3;

	// Populate a dkmrx matrix with the image data and write it out
	// to the output file using the standard dkmrx io interface.
	matrix mrx(0.0, winBmpInfoHeader.getHeight(), winBmpInfoHeader.getWidth());
	winBmpInfoHeader.wbmp2mrx(fileHeader.bfOffBits, inStream, mrx);
	char* strOutFileName = *(lplpszArgV + 2);
	writeMatrix(mrx, strOutFileName);
	cout << "Created output file " << strOutFileName << " successfully." << endl;

	return 0;
}

void writeMatrix(matrix& mrx, const char* strOutFileName)
{
	fstream file(strOutFileName, ios::out);
	// Allow to output 24 bits - 8 decimal digits
	file.width(14);
	file.precision(8);
	mrx.name("Contents of a Windows BMP file");
	file << mrx;
}

void printCopyright(void)
{
	cout << "winbmp2mrx, 64 bit, version 2.0" << endl;
#if defined(_CONSOLE )
	// DK, 8/Oct/2019. At the time of coding this function the default Windows
	// console setup doesn't print the Copyright unicode character properly
	// unless the 1252 code page is explicitly requested as shown below.
	SetConsoleOutputCP(1252);
	cout << "Copyright © 1994-2019 David Krikheli. All rights reserved." << endl;
#else
	cout << "Copyright (C) 1994-2019 David Krikheli. All rights reserved." << endl;
#endif
}

void printUsage(void)
{
	cout << "Usage:" << endl;
	cout << "    winbmp2mrx" << endl;
	cout << "    With no command line parameters the program prints this information." << endl;
	cout << endl;
	cout << "    winbmp2mrx <infile>" << endl;
	cout << "    Reads the header of the Windows Bitmap file specified by the infile parameter" << endl;
	cout << "    and prints the properties of the image." << endl;
	cout << endl;
	cout << "    winbmp2mrx <infile> <outfile>" << endl;
	cout << "    Reads the header of the Windows Bitmap file specified by the infile parameter" << endl;
	cout << "    and prints the properties of the image. Then reads on to get the image data" << endl;
	cout << "    and writes it to the output file specified by the outfile parameter using the" << endl;
	cout << "    dkmrx matrix data file format." << endl;
}
