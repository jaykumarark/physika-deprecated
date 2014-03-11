#include "image.h"


Image::Image(std::string filename)
{
	load(filename);
}



void Image::load(std::string filename)
{
	image = SOIL_load_image(filename.c_str(), &mwidth, &mheight, 0, SOIL_LOAD_RGB);
	mHeightMap = new float[mwidth*mheight];
}

int Image::width()
{
	return mwidth;
}

int Image::height()
{
	return mheight;
}

unsigned char* Image::data()
{
	return image;
}
Image::~Image(void)
{
	SOIL_free_image_data(image);
}


void Image::makeHeightMap()
{
	int k = 0;
	for(int i = 0 ; i < mwidth; i++)
	{
		for(int j = 0; j < mheight; j++)
		{
			mHeightMap[(j+(mheight*i))] = (float)image[k]/255;
			k+=3;
		}
	}
}

float Image::getColorAt(int x, int y)
{
	return mHeightMap[(y+(mheight*x))];
}