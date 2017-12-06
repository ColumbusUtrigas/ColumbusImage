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
		FILE* fp = fopen(file, "wb");
		if (fp == NULL) return false;

		png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (!png) return false;

		png_infop info = png_create_info_struct(png);
		if (!info)
		{
			png_destroy_write_struct(&png, &info);
			return false;
		}

		png_init_io(png, fp);
		int type = PNG_COLOR_TYPE_RGB;
		if (data.bpp == 4)
			type = PNG_COLOR_TYPE_RGBA;

		png_set_IHDR(png, info, data.w, data.h, 8, type, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		png_colorp palette = (png_colorp)png_malloc(png, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));
		if (!palette)
		{
			fclose(fp);
			png_destroy_write_struct(&png, &info);
			return false;
		}
		png_set_PLTE(png, info, palette, PNG_MAX_PALETTE_LENGTH);
		png_write_info(png, info);
		png_set_packing(png);

		png_bytepp rows = (png_bytepp)png_malloc(png, data.h * sizeof(png_bytep));
		int rowbytes = data.w * data.bpp;
		for (int i = 0; i < data.h; i++)
		{
			rows[i] = (png_bytep)malloc(rowbytes);
			memcpy(rows[i], data.data + (data.h - i - 1) * rowbytes, rowbytes);
		}

		png_write_image(png, rows);
		png_write_end(png, info);
		png_free(png, palette);
		png_destroy_write_struct(&png, &info);

		fclose(fp);
		for (int i = 0; i < data.h; i++)
		{
			free(rows[i]);
		}
		
		free(rows);
		return true;
	}

#ifdef __cplusplus
}
#endif







