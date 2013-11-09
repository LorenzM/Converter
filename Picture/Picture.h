#include <string>
#include <valarray>
#include <boost/iostreams/device/mapped_file.hpp> // for mmap
#include<iostream>
using namespace std;

class Picture
{
public:
	Picture(string file, int step);
	~Picture();
	//int * getPixel (int,int);
private:
	void FFillStructureWithPicInfo();
	void setPicPara();
	void setFrame();
	int readSingleValue();

	   //Attributes
	string fileName, magicNumber;
	int width, height, maxColorValue, width_frame, height_frame,step;
	//Pointer for mappedfile 
	const char* pointer_mmap;
	 const char* mmap_length ;
	// Matrix for PPM
	std::valarray<int> matrix_Red;
	std::valarray<int> matrix_Green;
	std::valarray<int> matrix_Blue;

	//TRGBContainer *bez;
};