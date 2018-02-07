#include "ColumbusImage.h"
#include "ClmbsImg_Util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tiff.h>
#include <tiffio.h>

#ifdef __cplusplus
extern "C"
{
#endif

	bool ClmbsImg_IsTIF(const char* file)
	{
		FILE* fp = fopen(file, "rb");
		if (file == NULL) return false;

		uint8_t magic[4];
		if (!ReadBytes(magic, sizeof(magic), fp)) return false;
		fclose(fp);

		bool II = (magic[0] == 'I' &&
			       magic[1] == 'I' &&
		           magic[2] == 42 &&
		           magic[3] == 0);

		bool MM = (magic[0] == 'M' &&
		           magic[1] == 'M' &&
		           magic[2] == 0 &&
		           magic[3] == 42);

		if (II || MM) return true;
		else return false;
	}

	ClmbsImg_Data ClmbsImg_LoadTIF(const char* file)
	{
		ClmbsImg_Data ret;

		TIFF* tif = TIFFOpen(file, "r");
		if (tif == NULL) return ret;

		unsigned int width = 0;
		unsigned int height = 0;
		unsigned int bpp = 0;

		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
		TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &bpp);

		ret.w = width;
		ret.h = height;
		ret.bpp = bpp;

		uint32_t* buffer = (uint32_t*)malloc(width * height * sizeof(uint32_t));
		TIFFReadRGBAImage(tif, width, height, buffer, 0);

		uint8_t* data = (uint8_t*)malloc(width * height * bpp);
		for (size_t i = 0; i < width * height * bpp; i += bpp)
		{
			data[i + 0] = TIFFGetR(*buffer);
			data[i + 1] = TIFFGetG(*buffer);
			data[i + 2] = TIFFGetB(*buffer);
			if (bpp == 4)
				data[i + 3] = TIFFGetA(*buffer);

			buffer++;
		}

		buffer -= width * height;
		free(buffer);

		TIFFClose(tif);

		ret.data = data;

		return ret;
	}

	bool ClmbsImg_SaveTIF(const char* file, ClmbsImg_Data data)
	{
		if (data.data == NULL) return false;

		TIFF* tif = TIFFOpen(file, "w");
		if (tif == NULL) return false;

		TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, data.w);
		TIFFSetField(tif, TIFFTAG_IMAGELENGTH, data.h);
		TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, data.bpp);
		TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
		TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

		TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
		TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
		TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_LZW);

		size_t stride = data.w * data.bpp;

		uint8_t* row = (uint8_t*)malloc(stride);
		uint32_t i;

		TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tif, stride));

		for (i = 0; i < data.h; i++)
		{
			memcpy(row, &data.data[(data.h - i - 1) * stride], stride);
			if (TIFFWriteScanline(tif, row, i, 0) < 0)
				break;
		}

		TIFFClose(tif);
		if (row) free(row);

		return true;
	}

#ifdef __cplusplus
}
#endif







