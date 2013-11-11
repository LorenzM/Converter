#include <vector>

class TRGBContainer;
class TYCbCrContainer;
class TBitStream;

int main();

class TJpegConverter
{
public:
	TJpegConverter();
	~TJpegConverter();

private:
	void FillStructureWithPicInfo();
	void ChangeColorModel();
	void SetPictureSizeIncludingStep();
	TRGBContainer* GetPixel(int x,int y);

	// In dieser Map können für jeden Pixel <waagerecht, senkrecht> eine beliebige Anzahl Pixel abgelegt werden.
	//std::map<int, std::map<int, TRGBContainer*>> FPictureMap;
	std::vector<TRGBContainer*> FPictureVec;
	std::vector<TYCbCrContainer*> FPictureVecYCbCr;

	std::string FPicWidth;
	std::string FPicHeight;

	std::string FPicWidthIncludingBorder;
	std::string FPicHeightIncludingBorder;
};

class TRGBContainer
{
public:
	// In dieser Struktur legen wir unsere Farbwerte ab.
	TRGBContainer(int R, int G, int B);

	// Hier werden die Farbwerte pro Container abgelegt.
	int FColorRed;
	int FColorGreen;
	int FColorBlue;
};

class TYCbCrContainer
{
public:
	// In dieser Struktur legen wir unsere Farbwerte ab.
	TYCbCrContainer(float Y, float Cb, float Cr);

	// Hier werden die Farbwerte pro Container abgelegt.
	int FColorY;
	int FColorCb;
	int FColorCr;
};

// Bitstrom Klassen
class TBitStream
{
public:
	TBitStream();
	void getBytes();
	void pushByte(unsigned char ABit);

private:
	typedef unsigned char byte;

	int FBitCounter;
	unsigned char FCurrentByte;

	FILE* FFile;
};
