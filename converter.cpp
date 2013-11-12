#include "Converter.h"
#include "BitStream.h"
#include <cstdio>
#include <string>
#include <vector>

using namespace std;

//-----------------------------------------------------------------------------------------------------
// Application Start
//-----------------------------------------------------------------------------------------------------
int main()
{
	//TJpegConverter* FJpegConv = new TJpegConverter();
	BitStream* bitStream = new BitStream();
	
	delete bitStream;	
	//delete FJpegConv;

	return 0;
}

//-----------------------------------------------------------------------------------------------------
// Jpeg Konvertierungs Klasse
//-----------------------------------------------------------------------------------------------------
TJpegConverter::TJpegConverter()
{
	// Datenstruktur befüllen
	FillStructureWithPicInfo();

	// Farbraum der gespeicherten Daten ändern
	ChangeColorModel();
}

void TJpegConverter::FillStructureWithPicInfo()
{
	errno_t error;

	FILE *input;
	error = fopen_s(&input, "Test.ppm", "rb");

	// --- Für Debug-Zwecke ---
	if (error == 0)
	{
		printf("Datei 'Test.ppm' erfolgreich geoeffnet. \n");
	}
	else
	{
		printf("Datei 'Test.ppm' NICHT erfolgreich geoeffnet. \n");
	}
	// --- ENDE ---

	if (input != NULL)
	{
		string value;
		string magic_number;
		
		// Das erste Zeichen auslesen
		value = fgetc(input);
		// Wir bauen hier in zwei Schritten die MagicNumber auf ~ erstmal besorgen wir uns das P
		magic_number = value;
		
		//... und danach die dazu gehörige Ziffer.
		value = fgetc(input);
		magic_number += value;

		printf("MagicNumber ausgelesen. \n");

		string color;
		
		fgetc(input);
		value = fgetc(input);
			
		if (value == "#")
		{
			// Wir ignorieren hier die Zeile mit der Raute ~ diese ist nämlich unbrauchbare Information!
			while (value != "\n")
				value = fgetc(input);

			// Breite des Bildes auslesen
			while (value != " ")
			{
				value = fgetc(input);

				if (value != " ")
					FPicWidth += value;
			}
			value = fgetc(input);

			// Höhe des Bildes auslesen
			while (value != " " && value != "\n")
			{
				if (value != " ")
					FPicHeight += value;

				value = fgetc(input);
			}
				
			// Nächstes NewLine überspringen
			fgetc(input);

			// Farbbereich auslesen
			while (value != "\n")
			{
				value = fgetc(input);

				if (value != "\n")
					color += value;
			}
		}

		string color_red;
		string color_green;
		string color_blue;
		int durchlaufzaehler = 1;
		int total_pixel_count = atoi(FPicWidth.c_str()) * atoi(FPicHeight.c_str());
		while (total_pixel_count >= FPictureVec.size())
		{
			value = fgetc(input);

			if (value == " " || value == "\n")
			{
				//Hiermit legen wir fest welchen Farbwert wir als nächstes auslesen.
				if (durchlaufzaehler < 4)
					++durchlaufzaehler;
			}
			else if (!feof(input) || durchlaufzaehler == 4)
			{
				if (durchlaufzaehler == 1)
				{
					// ROT
					color_red += value;
				}
				else if (durchlaufzaehler == 2)
				{
					// GRÜN
					color_green += value;
				}
				else if (durchlaufzaehler == 3)
				{
					// BLAU
					color_blue += value;
				}
				else if (durchlaufzaehler == 4)
				{
					// Wenn wir hier angekommen sind, haben wir alle 3 Farbwerte befüllt und können diese in unsere Datenhaltung speichern.
					FPictureVec.push_back(new TRGBContainer(atoi(color_red.c_str()), atoi(color_green.c_str()), atoi(color_blue.c_str())));

					// Wir haben ein neues Objekt erzeugt, jetzt müssen wir unseren Zähler auch zurücksetzten!
					durchlaufzaehler = 1;

					// Zurücksetzten nicht vergessen.
					color_red = "";
					color_green = "";
					color_blue = "";

					// erste Ziffer von ROT
					color_red += value;
				}
			}
		}

		printf("Datenstruktur befuellt. \n");
	}	
}

void TJpegConverter::ChangeColorModel()
{
	for (std::vector<TRGBContainer*>::iterator i = FPictureVec.begin(); i!=FPictureVec.end(); i++)
	{
		float y = ((*i)->FColorRed * 0.299) + ((*i)->FColorGreen * 0.587) + ((*i)->FColorBlue * 0.114);
		float cb = 128 + ((*i)->FColorRed * -0.1687) + ((*i)->FColorGreen * -0.3312) + ((*i)->FColorBlue * 0.5);
		float cr = 128 + ((*i)->FColorRed * 0.5) + ((*i)->FColorGreen * -0.4186) + ((*i)->FColorBlue * -0.0813);
		
		FPictureVecYCbCr.push_back(new TYCbCrContainer(y, cb, cr));
	}
}

TRGBContainer* TJpegConverter::GetPixel(int x,int y)
{
	int original_width = atoi(FPicWidth.c_str());
	int original_height = atoi(FPicHeight.c_str());

	int actual_width = atoi(FPicWidthIncludingBorder.c_str());
	int actual_height = atoi(FPicHeightIncludingBorder.c_str());

	if (x < actual_width && y < actual_height )
	{//In Frame

		if(x > original_width-1)
		{//X in frame. Out of picture. Take Values from last Pixel
			x = original_width-1;
		}

		if(y > original_height-1)
		{//Y in frame. Out of picture.
			y = original_height-1;
		}
		//change mg
		TRGBContainer * tpixel;//Pointer auf Pixel im Vektor		

		int position =  y * (original_width) + x; //Pionter zeigt auf Pixel 1 (0,0) 
		vector<TRGBContainer*>::iterator pit=FPictureVec.begin();
		pit=pit+position;
		tpixel=*pit;
		//end change mg
		return tpixel;
	}
	else
	{
		return 0;//Out of Frame. You are lost!!
	}
	return 0;
}

void TJpegConverter::SetPictureSizeIncludingStep()
{
	int original_width = atoi(FPicWidth.c_str());
	int original_height = atoi(FPicHeight.c_str());

	int actual_width = atoi(FPicWidthIncludingBorder.c_str());
	int actual_height = atoi(FPicHeightIncludingBorder.c_str());

	if(original_width > actual_width)
	{
		if (original_width % actual_width == 0)
			actual_width = original_width;
		else
			actual_width = (original_width/actual_width+1)*actual_width;
	}

	if(original_height > actual_height)
	{
		if (original_height % actual_height == 0)
			actual_height = original_height;
		else
			actual_height = (original_height/actual_height+1)*actual_height;
	}

	FPicWidthIncludingBorder = actual_width;
	FPicHeightIncludingBorder = actual_height;
}

TJpegConverter::~TJpegConverter()
{

}

//-----------------------------------------------------------------------------------------------------
// RGB-Farben Container Klasse
//-----------------------------------------------------------------------------------------------------
TRGBContainer::TRGBContainer(int R, int G, int B)
{
	FColorRed = R;
	FColorGreen = G;
	FColorBlue = B;
}

//-----------------------------------------------------------------------------------------------------
// YCbCr-Farben Container Klasse
//-----------------------------------------------------------------------------------------------------
TYCbCrContainer::TYCbCrContainer(float Y, float Cb, float Cr)
{
	FColorY = Y;
	FColorCb = Cb;
	FColorCr = Cr;
}
