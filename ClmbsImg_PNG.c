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

		FILE *fp = fopen(file, "rb");
		if (fp == NULL) return ret;

		png_structp png_ptr;
		png_infop info_ptr;
		unsigned int sig_read = 0;
		int color_type, interlace_type;

		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

		if (png_ptr == NULL)
		{
			fclose(fp);
			return ret;
		}

		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL)
		{
			fclose(fp);
			png_destroy_read_struct(&png_ptr, NULL, NULL);
			return ret;
		}

		if (setjmp(png_jmpbuf(png_ptr)))
		{
			png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
			fclose(fp);
			return ret;
		}

		png_init_io(png_ptr, fp);
		png_set_sig_bytes(png_ptr, sig_read);
		png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

		png_uint_32 width, height;
		int bit_depth;
		png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		&interlace_type, NULL, NULL);

		unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);

		ret.w = width;
		ret.h = height;
		ret.bpp = bit_depth / 2;
		ret.data = (uint8_t*)malloc(row_bytes * ret.h);

		png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

		for (int i = 0; i < ret.h; i++)
		{
			memcpy(ret.data + row_bytes * (ret.h - i), row_pointers[i], row_bytes);
		}

		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

		fclose(fp);

		return ret;
	}

#ifdef __cplusplus
}
#endif







