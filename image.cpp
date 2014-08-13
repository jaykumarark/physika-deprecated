#include "image.h"


Image::Image(std::string filename, bool needHeightMap)
{
	load(filename);
	if(needHeightMap)
	{
		makeHeightMap();
	}
}

Image::~Image(void)
{
	SOIL_free_image_data(mData);
}

void Image::load(std::string filename)
{
	mData = SOIL_load_image(filename.c_str(), &mwidth, &mheight, 0, SOIL_LOAD_RGB);
	if(mData == NULL)
	{
		std::cerr<<"Could not load image "<<filename.c_str()<<std::endl; 
		std::cout<<"Exiting... "<<std::endl;
		std::cout<<"Press any key to exit."<<std::endl;
		std::cin.get();
		std::exit(EXIT_FAILURE);
	}
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
	return mData;
}

void Image::makeHeightMap()
{
	int k = 0;
	for(int i = 0 ; i < mwidth; i++)
	{
		for(int j = 0; j < mheight; j++)
		{
			float val = (float)mData[k]/255;
			val = std::max(0.f, std::min(val, 1.f));
			mHeightMap[(j+(mheight*i))] = val;
			k+=3;
		}
	}
}

float Image::getColorAt(int x, int y)
{
	return std::max(0.f, std::min(mHeightMap[(y+(mheight*x))], 1.f));
}