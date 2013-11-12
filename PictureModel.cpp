#include "PictureModel.h"


PictureModel::PictureModel() :	yLayer(0), cbLayer(0), crLayer(0),
							yLayerSize(0), cbLayerSize(0), crLayerSize(0), 
							height(0), width(0),
							identifier("JFIF"), version("\1\1"),
							pixleUnit(0x00)
{
	pixleDensitiy_x[0] = 0x00;
	pixleDensitiy_x[1] = 0x48;
	pixleDensitiy_y[0] = 0x00;
	pixleDensitiy_y[1] = 0x48;
}

PictureModel::~PictureModel()
{
	delete[] yLayer, cbLayer, crLayer;
}
