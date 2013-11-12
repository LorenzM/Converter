#include "BitStream.h"

using namespace std;

//-----------------------------------------------------------------------------------------------------
// BitStream Klasse
//-----------------------------------------------------------------------------------------------------
BitStream::BitStream() :
	FCurrentByte(0),
	FBitCounter(0)
{
	// Das "wb" steht hier für 'w' = write -> Also neue Datei anlegen UND 'b' = binary -> Also als Binärdatei öffnen
	fopen_s(&FFile, "myfile.bin", "wb");

	// Geschwindigkeitstest beim Schreiben von 10.000.000 Bit
	/*for (long i = 0; i<3333334; ++i)
	{
		pushBit(1);
		pushBit(0);
		pushBit(1);
	}*/

	// Vorläufige Lösung - das PictureModel muss noch mit Werten des geladenen Bildes befüllt werden. Aktuell stehen hier noch feste Werte!
	FPicModel = new PictureModel();

	// Schreiben der JPG Segmente
	writeToStream_SOI();
	writeToStream_APP0();
	writeToStream_SOF0();
	writeToStream_EOI();
				
	// Neu erzeugtes File schliessen...
	fclose(FFile);

	// ... damit wir es zum lesen erneut öffnen können.
	fopen_s(&FFile, "myfile.bin", "rb");

	getBytes();

	fclose(FFile);
}

void BitStream::writeToStream_SOI()
{
	//SOI
	pushByte(0xff);
	pushByte(0xd8);
}

void BitStream::writeToStream_APP0()
{
	//APP0
	pushByte(0xff);
	pushByte(0xe0);
	pushByte(0x00);
	pushByte(0x10);
	//stream->pushBytes(FPicModel->identifier);
	//0x4a, 0x46, 0x49, 0x46, 0x00 "JFIF"
	pushByte(0x4a);//J
	pushByte(0x46);//F
	pushByte(0x49);//I
	pushByte(0x46);//F
	pushByte(0x00);//0
	//end JFIF
		
	//stream->pushBytes(FPicModel->version);
	pushByte(0x01);//1 Version
	pushByte(0x01);//1
	pushByte(0x00);//pixelunit
	pushByte(FPicModel->pixleDensitiy_x[0]);
	pushByte(FPicModel->pixleDensitiy_x[1]);
	pushByte(FPicModel->pixleDensitiy_y[0]);
	pushByte(FPicModel->pixleDensitiy_y[1]);
	pushByte(0x00);//kein Vorschaubild x
	pushByte(0x00);;//kein Vorschaubild y
	//pushByte(0x00);
}

void BitStream::writeToStream_SOF0()
{
	//SOFO
	pushByte(0xff);
	pushByte(0xc0);
	//Länge 17 (8+Komponenten*3)
	pushByte(0x00);
	pushByte(0x11);
	//Genauigkeit der Daten
	pushByte(0x08);
	//Bild größe x (high, low)
	pushByte((unsigned char)(FPicModel->height >> 8));
	pushByte((unsigned char)(FPicModel->height));
	//Bild größe y (high, low)
	pushByte((unsigned char)(FPicModel->width >> 8));
	pushByte((unsigned char)(FPicModel->width));
	pushByte(0x03);//Komponenten
	//3Byte pro Komponente
	pushByte(0x01);
	pushByte(0x22);
	pushByte(0x01);

	pushByte(0x02);
	pushByte(0x11);
	pushByte(0x01);

	pushByte(0x03);
	pushByte(0x11);
	pushByte(0x01);
}

void BitStream::writeToStream_EOI()
{
	//EOI
	pushByte(0xff);
	pushByte(0xd9);
}

void BitStream::getBytes()
{
	string buffer;
	
	// Jedes Byte einzeln auslesen und in eine Stringliste schreiben
	while (!feof(FFile))
		buffer += fgetc(FFile);

	/*
	// Größe der Datei
	fseek (FFile, 0, SEEK_END);
	long size = ftell(FFile);
	rewind (FFile);

	for (int i = 1; i<=size; ++i)
	{
		fread(&buffer,1,size,FFile);
	}*/
}

void BitStream::pushBit(unsigned char ABit)
{
	// Hier bauen wir im FCurrentByte solange unsere Bits zusammen, bis ein Byte dabei herauskommt.
	// '<<=' ist nichts anderes als ein Links-Shift! (Bsp.: 0001 -> 0010)
	FCurrentByte <<= 1;
	// '|=' hiermit verknüpfen wir die Bits von FCurrentByte mit ABit. (Bsp.: 1100 | 1010 -> 1110)
	FCurrentByte |= ABit;

	// Bei 8 Bit ist logischerweise ein Byte befüllt
	if(++FBitCounter == 8)
	{
		// Byteweise in unser File schreiben.
		fwrite(&FCurrentByte,1,1,FFile);
		FBitCounter = 0;
		FCurrentByte = 0;
	}
}

void BitStream::pushByte(unsigned char AByte)
{
	// Byteweise in unser File schreiben.
	fwrite(&AByte,1,1,FFile);
}
