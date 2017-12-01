#include "ColumbusImage.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

	static int ReadBytes(void* data, size_t size, FILE* fp)
	{
		if (fread(data, size, 1, fp) != 1) return 0;
		return 1;
	}

	static int WriteBytes(const void* data, size_t size, FILE* fp)
	{
		if (fwrite(data, size, 1, fp) != 1) return 0;
		return 1;
	}

	static int ReadUint8(uint8_t* data, FILE* fp)
	{
		return ReadBytes(data, sizeof(uint8_t), fp);
	}

	static int WriteUint8(const uint8_t* data, FILE* fp)
	{
		return WriteBytes(data, sizeof(uint8_t), fp);
	}

	static int ReadUint16(uint16_t* data, FILE* fp)
	{
		return ReadBytes(data, sizeof(uint16_t), fp);
	}

	static int WriteUint16(const uint16_t* data, FILE* fp)
	{
		return WriteBytes(data, sizeof(uint16_t), fp);
	}

	static int ReadUint32(uint32_t* data, FILE* fp)
	{
		return ReadBytes(data, sizeof(uint32_t), fp);
	}

	static int WriteUint32(const uint32_t* data, FILE* fp)
	{
		return WriteBytes(data, sizeof(uint32_t), fp);
	}

	static int ReadInt32(int32_t* data, FILE* fp)
	{
		return ReadBytes(data, sizeof(int32_t), fp);
	}

	static int WriteInt32(const int32_t* data, FILE* fp)
	{
		return WriteBytes(data, sizeof(int32_t), fp);
	}

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

	typedef struct
	{
		TGA_HEADER header;

		uint8_t* image_identification_field;
		uint8_t* color_map_data;
		uint8_t* image_data;
	} TGA_FILE;

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

	static bool Decode24(uint8_t* data, size_t size)
	{
		uint8_t bgr[3];

		for (int i = 0; i < size; i += 3)
		{
			bgr[0] = data[i + 0];
			bgr[1] = data[i + 1];
			bgr[2] = data[i + 2];

			data[i + 0] = bgr[2];
			data[i + 1] = bgr[1];
			data[i + 2] = bgr[0];
		}

		return true;
	}

	static bool Decode(uint8_t* data, size_t size, size_t bits)
	{
		switch (bits)
		{
		case 24:
			return Decode24(data, size);
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

		TGA_FILE tga;

		if (!ReadHeader(&tga.header, fp)) return ret;

		fclose(fp);
		return ret;
	}

	bool ClmbsImg_SaveTGA(const char* file, ClmbsImg_Data data)
	{
		FILE* fp = fopen(file, "wb");
		if (fp == NULL) return false;

		uint8_t header[18]={0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		header[12] = data.w & 0xFF;
		header[13] = (data.w >> 8) & 0xFF;
		header[14] = (data.h) & 0xFF; 
		header[15] = (data.h >> 8)  & 0xFF;
		header[16] = 24;

		uint8_t* buffer = (uint8_t*)malloc(data.w * data.h * data.bpp);
		memcpy(buffer, data.data, data.w * data.h * data.bpp);
		Decode(buffer, data.w * data.h * data.bpp, data.bpp * 8);

		fwrite(header, sizeof(uint8_t), 18, fp);
		fwrite(buffer, data.w * data.h * data.bpp, 1, fp);

		fclose(fp);
		free(buffer);
		return true;
	}

#ifdef __cplusplus
}
#endif








