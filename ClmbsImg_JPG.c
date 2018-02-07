#include "ColumbusImage.h"
#include "ClmbsImg_Util.h"
#include <jpeglib.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

	bool ClmbsImg_IsJPG(const char* file)
	{
		FILE* fp = fopen(file, "rb");
		if (fp == NULL) return false;

		uint8_t magic[4];
		fseek(fp, 6, SEEK_CUR);
		if (!ReadBytes(magic, sizeof(magic), fp)) return false;

		if (magic[0] == 'J' &&
			magic[1] == 'F' &&
			magic[2] == 'I' &&
			magic[3] == 'F') return true;

		return false;
	}

	ClmbsImg_Data ClmbsImg_LoadJPG(const char* file)
	{
		ClmbsImg_Data ret;

		struct jpeg_decompress_struct cinfo;

		struct jpeg_error_mgr pub;
		jmp_buf setjmp_buffer;

		FILE* fp = fopen(file, "rb");
		JSAMPARRAY buffer;
		size_t row_stride;

		if (fp == NULL) return ret;

		cinfo.err = jpeg_std_error(&pub);
		
		if (setjmp(setjmp_buffer))
		{
			jpeg_destroy_decompress(&cinfo);
			fclose(fp);
			return ret;
		}

		jpeg_create_decompress(&cinfo);

		jpeg_stdio_src(&cinfo, fp);

		jpeg_read_header(&cinfo, TRUE);

		ret.w = cinfo.image_width;
		ret.h = cinfo.image_height;
		ret.bpp = 3;

		jpeg_start_decompress(&cinfo);

		row_stride = cinfo.output_width * cinfo.output_components;
		buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

		uint8_t* data = (uint8_t*)malloc(cinfo.image_width * cinfo.image_height * 3);
		uint64_t counter = 0;
		uint64_t maxsize = row_stride * cinfo.image_height;

		while (cinfo.output_scanline < cinfo.output_height)
		{
			jpeg_read_scanlines(&cinfo, buffer, 1);
			memcpy(data + (maxsize - counter - row_stride), buffer[0], row_stride);
			counter += row_stride;
		}

		jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);

		fclose(fp);

		ret.data = data;

		return ret;
	}

	bool ClmbsImg_SaveJPG(const char* file, ClmbsImg_Data data, unsigned int quality)
	{
		if (data.data == NULL) return false;

		struct jpeg_compress_struct cinfo;
		struct jpeg_error_mgr jerr;
		
		FILE* fp = fopen(file, "wb");;
		if (fp == NULL) return false;

		JSAMPROW row_pointer[1];
		size_t row_stride;

		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_compress(&cinfo);

		jpeg_stdio_dest(&cinfo, fp);

		cinfo.image_width = data.w;
		cinfo.image_height = data.h;
		cinfo.input_components = data.bpp;
		cinfo.in_color_space = JCS_RGB;
		jpeg_set_defaults(&cinfo);
		jpeg_set_quality(&cinfo, quality, TRUE);

		jpeg_start_compress(&cinfo, TRUE);

		row_stride = data.w * data.bpp;
		uint64_t maxsize = row_stride * data.h;

		while (cinfo.next_scanline < cinfo.image_height)
		{
			row_pointer[0] = (JSAMPROW)&data.data[maxsize - cinfo.next_scanline * row_stride - row_stride];
			jpeg_write_scanlines(&cinfo, row_pointer, 1);
		}

		jpeg_finish_compress(&cinfo);
		fclose(fp);
		jpeg_destroy_compress(&cinfo);

		return true;
	}

#ifdef __cplusplus
}
#endif





