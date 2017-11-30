#pragma once

#include <cstdlib>
#include <cstdio>
#include "ImageTypes.h"

namespace Columbus
{

	bool IsBMP(const char* file);
	ImageData LoadBMP(const char* file);

	bool IsBMP(const char* file)
	{
		FILE* fp = fopen(file, "rb");
		if (fp == NULL)
			{ return false; }

		unsigned char magic[2];
		fread(magic, sizeof(magic), 1, fp);
		fclose(fp);

		if (magic[0] == 'B' && magic[1] == 'M')
			return true;
		else
			return false;
	}

	ImageData LoadBMP(const char* file)
	{
		ImageData ret;

	    FILE* fp = fopen(file, "rb");
	    if (fp == NULL) return ret;

	    unsigned char info[54];
	    fread(info, sizeof(info), 1, fp);

	    int width = *(int*)&info[18];
	    int height = *(int*)&info[22];

	    int size = 3 * width * height;
	    unsigned char* data = new unsigned char[size];
	    fread(data, sizeof(unsigned char), size, fp);
	    fclose(fp);

	   	for(size_t i = 0; i < size; i += 3)
	    {
            unsigned char tmp = data[i];
            data[i] = data[i + 1];
            data[i + 1] = tmp;
	    }

	    ret.w = width;
	    ret.h = height;
	    ret.bpp = 3;
	    ret.data = data;

		return ret;
	}

}





