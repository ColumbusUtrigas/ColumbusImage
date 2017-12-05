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

	static bool Decode(uint8_t* data, size_t size, size_t bits)
	{
		switch (bits)
		{
		case 24:
			return bgr2rgb(data, size);
			break;
		};

		return true;
	}

	bool ClmbsImg_IsTGA(const char* file)
	{
		return true;
	}

	ClmbsImg_Data ClmbsImg_LoadTGA(const char* file)
	{
		ClmbsImg_Data ret;

		FILE* fp = fopen(file, "rb");
		if (fp == NULL) return ret;

		TGA_HEADER tga;

		if (!ReadHeader(&tga, fp)) return ret;

		size_t size = tga.width * tga.height * tga.bits / 8;

		uint8_t* buffer = (uint8_t*)malloc(size);

		if (tga.image_type == 2)
		{
			fread(buffer, size, 1, fp);
			Decode(buffer, size, tga.bits);
		}

		ret.w = tga.width;
		ret.h = tga.height;
		ret.bpp = tga.bits / 8;
		ret.data = buffer;

		fclose(fp);
		return ret;
	}

	bool ClmbsImg_SaveTGA(const char* file, ClmbsImg_Data data)
	{
		FILE* fp = fopen(file, "wb");
		if (fp == NULL) return false;

		TGA_HEADER tga = {0, 0, 2, 0, 0, 0, 0, 0, data.w, data.h, data.bpp * 8, 0};


		uint8_t* buffer = (uint8_t*)malloc(data.w * data.h * data.bpp);
		memcpy(buffer, data.data, data.w * data.h * data.bpp);
		Decode(buffer, data.w * data.h * data.bpp, data.bpp * 8);

		WriteHeader(tga, fp);
		fwrite(buffer, data.w * data.h * data.bpp, 1, fp);

		fclose(fp);
		free(buffer);
		return true;
	}

#ifdef __cplusplus
}
#endif








