#include "PictureModel.h"
#include <cstdio>
#include <string>

// BitStream Klasse
class BitStream
{
public:
	BitStream();

	void getBytes();
	// Methode um Bits einzeln zu schreiben.
	void pushBit(unsigned char ABit);
	// Methode zum schreiben von Bytes.
	void pushByte(unsigned char AByte);

	void writeToStream_SOI();
	void writeToStream_APP0();
	void writeToStream_SOF0();
	void writeToStream_EOI();

	PictureModel* FPicModel;

private:
    int FBitCounter;
    unsigned char FCurrentByte;

    FILE* FFile;
};
