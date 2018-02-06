#include "ColumbusImage.h"
#include "ClmbsImg_Util.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

	#define READPIXEL24(a) \
		blue = *a++; \
		green = *a++; \
		red = *a++;

	#define READPIXEL32(a) \
		READPIXEL24(a) \
		alpha = *a++;

	#define WRITEPIXEL24(a) \
		*a++ = red; \
		*a++ = green; \
		*a++ = blue;

	#define WRITEPIXEL32(a) \
		WRITEPIXEL24(a) \
		*a++ = alpha;

	typedef struct
	{
		uint8_t idlen; //Image ID Lenght
		uint8_t color_map_type; //Color map type
		uint8_t image_type; //Image Type
		/*
		0  -  No image data included.
		1  -  Uncompressed, color-mapped images.
		2  -  Uncompressed, RGB images.
		3  -  Uncompressed, black and white images.
		9  -  Runlength encoded color-mapped images.
		10  -  Runlength encoded RGB images.
		11  -  Compressed, black and white images.
		32  -  Compressed color-mapped data, using Huffman, Delta, and runlength encoding.
		33  -  Compressed color-mapped data, using Huffman, Delta, and runlength encoding.  4-pass quadtree-type process.
		*/
		uint16_t color_map_origin;    //Color
		uint16_t color_map_length;    //map
		uint8_t color_map_entry_size; //specification

		uint16_t x_origin; //
		uint16_t y_origin; //Image
		uint16_t width;    //specification
		uint16_t height;   //
		uint8_t bits; // Bit depth. 8, 15, 16, 24 or 32
		uint8_t image_descriptor;
	} TGA_HEADER;

	static bool ReadHeader(TGA_HEADER* header, FILE* fp)
	{
		if (header == NULL || fp == NULL) return false;

		if (!ReadUint8(&header->idlen, fp)) return false;
		if (!ReadUint8(&header->color_map_type, fp)) return false;
		if (!ReadUint8(&header->image_type, fp)) return false;
		if (!ReadUint16(&header->color_map_origin, fp)) return false;
		if (!ReadUint16(&header->color_map_length, fp)) return false;
		if (!ReadUint8(&header->color_map_entry_size, fp)) return false;
		if (!ReadUint16(&header->x_origin, fp)) return false;
		if (!ReadUint16(&header->y_origin, fp)) return false;
		if (!ReadUint16(&header->width, fp)) return false;
		if (!ReadUint16(&header->height, fp)) return false;
		if (!ReadUint8(&header->bits, fp)) return false;
		if (!ReadUint8(&header->image_descriptor, fp)) return false;

		return true;
	}

	static bool WriteHeader(TGA_HEADER header, FILE* fp)
	{
		if (fp == NULL) return false;

		if (!WriteUint8(&header.idlen, fp)) return false;
		if (!WriteUint8(&header.color_map_type, fp)) return false;
		if (!WriteUint8(&header.image_type, fp)) return false;
		if (!WriteUint16(&header.color_map_origin, fp)) return false;
		if (!WriteUint16(&header.color_map_length, fp)) return false;
		if (!WriteUint8(&header.color_map_entry_size, fp)) return false;
		if (!WriteUint16(&header.x_origin, fp)) return false;
		if (!WriteUint16(&header.y_origin, fp)) return false;
		if (!WriteUint16(&header.width, fp)) return false;
		if (!WriteUint16(&header.height, fp)) return false;
		if (!WriteUint8(&header.bits, fp)) return false;
		if (!WriteUint8(&header.image_descriptor, fp)) return false;

		return true;
	}

	bool ClmbsImg_IsTGA(const char* file)
	{
		return true;
	}

	static void RGBCompressedTGA(uint8_t* aIn, uint8_t* aOut, size_t aSize)
	{
		int header;
		int blue, green, red;
		uint8_t* pixel = (uint8_t*)malloc(3);
		size_t i, j, pixelcount;

		for (i = 0; i < aSize; )
		{
			header = *aIn++;
			pixelcount = (header & 0x7f) + 1;

			if (header & 0x80)
			{
				READPIXEL24(aIn)
				for (j = 0; j < pixelcount; j++)
				{
					WRITEPIXEL24(aOut)
				}
				i += pixelcount;
			} else
			{
				for (j = 0; j < pixelcount; j++)
				{
					READPIXEL24(aIn)
					WRITEPIXEL24(aOut)
				}
				i += pixelcount;
			}
		}
	}

	static void RGBACompressedTGA(uint8_t* aIn, uint8_t* aOut, size_t aSize)
	{
		int header;
		int blue, green, red, alpha;
		int pix;
		size_t i, j, pixelcount;

		for (i = 0; i < aSize; )
		{
			header = *aIn++;
			pixelcount = (header & 0x7f) + 1;
			if (header & 0x80)
			{
				READPIXEL32(aIn);
				pix = red | (green << 8) | (blue << 16) | (alpha << 24);

				for (j = 0; j < pixelcount; j++)
				{
					memcpy(aOut, &pix, 4);
					aOut += 4;
				}

				i += pixelcount;
			}
			else
			{
				for (j = 0; j < pixelcount; j++)
				{
					READPIXEL32(aIn)
					WRITEPIXEL32(aOut)
				}
				i += pixelcount;
			}
		}
	}

	ClmbsImg_Data ClmbsImg_LoadTGA(const char* file)
	{
		ClmbsImg_Data ret;

		FILE* fp = fopen(file, "rb");
		if (fp == NULL) return ret;

		TGA_HEADER tga;

		if (!ReadHeader(&tga, fp)) return ret;

		size_t offset = ftell(fp);
		fseek(fp, 0, SEEK_END);
		size_t s = ftell(fp);
		fseek(fp, offset, SEEK_SET);

		size_t dSize = s - sizeof(TGA_HEADER);
		size_t size = tga.width * tga.height * tga.bits / 8;

		uint8_t* buffer = (uint8_t*)malloc(dSize);
		fread(buffer, dSize, 1, fp);

		uint8_t* data = NULL;

		switch (tga.image_type)
		{
		case 2:
			//Uncompressed RGB
			data = buffer;
			if (tga.bits == 24)
				bgr2rgb(buffer, size);
			else
				bgra2rgba(buffer, size);
			break;
		case 10:
			//Compressed RGB
			data = (uint8_t*)malloc(size);
			if (tga.bits == 24)
				RGBCompressedTGA(buffer, data, tga.width * tga.height);
			else
				RGBACompressedTGA(buffer, data, tga.width * tga.height);
			break;
		}

		if (tga.x_origin != 0)
			flipX(buffer, tga.width, tga.height, tga.bits / 8);

		if (tga.y_origin != 0)
			flipY(data, tga.width, tga.height, tga.bits / 8);

		fclose(fp);

		ret.w = tga.width;
		ret.h = tga.height;
		ret.bpp = tga.bits / 8;
		ret.data = data;

		return ret;
	}

	bool ClmbsImg_SaveTGA(const char* file, ClmbsImg_Data data)
	{
		FILE* fp = fopen(file, "wb");
		if (fp == NULL) return false;

		TGA_HEADER tga = {0, 0, 2, 0, 0, 0, 0, 0, data.w, data.h, data.bpp * 8, 8};

		size_t size = data.w * data.h * data.bpp;

		uint8_t* buffer = (uint8_t*)malloc(size);
		memcpy(buffer, data.data, size);

		if (tga.bits == 24)
			rgb2bgr(buffer, size);

		if (tga.bits == 32)
			rgba2bgra(buffer, size);

		WriteHeader(tga, fp);
		fwrite(buffer, data.w * data.h * data.bpp, 1, fp);

		fclose(fp);
		free(buffer);
		return true;
	}

#undef READPIXEL24
#undef WRITEPIXEL24
#undef WRITEPIXEL32

#ifdef __cplusplus
}
#endif








