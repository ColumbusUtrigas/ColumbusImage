#include "ColumbusImage.h"
#include "ClmbsImg_Util.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <png.h> //LibPNG header

#ifdef __cplusplus
extern "C"
{
#endif

	bool ClmbsImg_IsPNG(const char* file)
	{
		FILE* fp = fopen(file, "rb");
		if (fp == NULL) return false;

		uint8_t magic[4];
		if (!ReadBytes(magic, sizeof(magic), fp)) return false;
		fclose(fp);

		if (magic[1] == 'P' &&
		    magic[2] == 'N' &&
		    magic[3] == 'G') return true;
		else return false;
	}

	ClmbsImg_Data ClmbsImg_LoadPNG(const char* file)
	{
		ClmbsImg_Data ret;

		FILE* fp = fopen(file, "rb");
		if (fp == NULL) return ret;

		png_structp	png_ptr;
		png_infop info_ptr;
		png_uint_32 width;
		png_uint_32 height;
		int bit_depth;
		int color_type;
		int interlace_method;
		int compression_method;
		int filter_method;

		png_bytepp rows;

		png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (! png_ptr) return ret;

		info_ptr = png_create_info_struct (png_ptr);
		if (! png_ptr) return ret;

		png_init_io (png_ptr, fp);
		png_read_png (png_ptr, info_ptr, 0, 0);
		png_get_IHDR (png_ptr, info_ptr, &width, &height, &bit_depth,
		&color_type, &interlace_method, &compression_method, &filter_method);

		rows = png_get_rows (png_ptr, info_ptr);
		int rowbytes;
		rowbytes = png_get_rowbytes (png_ptr, info_ptr);

		ret.w = width;
		ret.h = height;
		if (color_type == PNG_COLOR_TYPE_RGB)
			ret.bpp = 3;
		if (color_type == PNG_COLOR_TYPE_RGBA)
			ret.bpp = 4;
		ret.data = (uint8_t*)malloc(ret.w * ret.h * ret.bpp);;

		for (int i = 0; i < height; i++)
		{
			memcpy(ret.data + rowbytes * i, rows[height - i - 1], rowbytes);
			free(rows[i]);
		}

		if (png_ptr != NULL)
			free(png_ptr);

		if (info_ptr != NULL)
			free(info_ptr);

		fclose(fp);
		free(rows);

		return ret;
	}

	bool ClmbsImg_SavePNG(const char* file, ClmbsImg_Data data)
	{
		return true;
	}

#ifdef __cplusplus
}
#endif







