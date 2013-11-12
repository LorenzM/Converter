#ifndef PICTUREMODEL_H
#define PICTUREMODEL_H

#include <string>

using std::string;

class PictureModel
{
public:
	PictureModel();
	~PictureModel();
	unsigned int yLayerSize, cbLayerSize, crLayerSize;
	unsigned int height, width;
	unsigned short *yLayer;
	unsigned short *cbLayer;
	unsigned short *crLayer;
	string identifier;
	string version;
	unsigned char pixleUnit;
	unsigned char pixleDensitiy_x[2];
	unsigned char pixleDensitiy_y[2];

private:
};
#endif
