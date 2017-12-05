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
		uint8_t magic[2]; //Magic Bytes 'B' and 'M'
		uint32_t size; //Size of whole file
		uint32_t unused; //Should be 0
		uint32_t offset; //Offset to bitmap data
	} BMP_HEADER;

	typedef struct
	{
		uint32_t infosize; //Size of info struct (40 bytes)
		int32_t width; //Width of image
		int32_t height; //Height of image
		uint16_t planes; //Should be 1
		uint16_t bits; //Bits per pixel (1, 4, 8, 16, 24, 32)
		uint32_t compression; //0 = none, 1 = 8-bit RLE, 2 = 4-bit RLE
		uint32_t size_data; //The image size
		uint32_t hres; //Horizontal resolution (pixel per meter)
		uint32_t vres; //Vertical resolution (pixel per meter)
		uint32_t colors; //Number of palette colors
		uint32_t important_colors; //Number of important colors;
	} BMP_INFO;

	typedef struct
	{
		uint8_t b; //Blue channel
		uint8_t g; //Green channel
		uint8_t r; //Red channel
		uint8_t a; //Alpha channel
	} BMP_PALETTE;

	static bool ReadHeader(BMP_HEADER* header, FILE* fp)
	{
		if (header == NULL || fp == NULL) return false;

		if (!ReadUint8(&header->magic[0], fp)) return false;
		if (!ReadUint8(&header->magic[1], fp)) return false;
		if (!ReadUint32(&header->size, fp)) return false;
		if (!ReadUint32(&header->unused, fp)) return false;
		if (!ReadUint32(&header->offset, fp)) return false;

		return true;
	}

	static bool WriteHeader(BMP_HEADER header, FILE* fp)
	{
		if (fp == NULL) return false;

		if (!WriteUint8(&header.magic[0], fp)) return false;
		if (!WriteUint8(&header.magic[1], fp)) return false;
		if (!WriteUint32(&header.size, fp)) return false;
		if (!WriteUint32(&header.unused, fp)) return false;
		if (!WriteUint32(&header.offset, fp)) return false;

		return true;
	}

	static bool ReadInfo(BMP_INFO* info, FILE* fp)
	{
		if (info == NULL || fp == NULL) return false;

		if (!ReadUint32(&info->infosize, fp)) return false;
		if (!ReadInt32(&info->width, fp)) return false;
		if (!ReadInt32(&info->height, fp)) return false;
		if (!ReadUint16(&info->planes, fp)) return false;
		if (!ReadUint16(&info->bits, fp)) return false;
		if (!ReadUint32(&info->compression, fp)) return false;
		if (!ReadUint32(&info->size_data, fp)) return false;
		if (!ReadUint32(&info->hres, fp)) return false;
		if (!ReadUint32(&info->vres, fp)) return false;
		if (!ReadUint32(&info->colors, fp)) return false;
		if (!ReadUint32(&info->important_colors, fp)) return false;

		uint8_t* empty = (uint8_t*)malloc(68);
		ReadBytes(empty, 68, fp);
		free(empty);

		return true;
	}

	static bool WriteInfo(BMP_INFO info, FILE* fp)
	{
		if (fp == NULL) return false;

		if (!WriteUint32(&info.infosize, fp)) return false;
		if (!WriteInt32(&info.width, fp)) return false;
		if (!WriteInt32(&info.height, fp)) return false;
		if (!WriteUint16(&info.planes, fp)) return false;
		if (!WriteUint16(&info.bits, fp)) return false;
		if (!WriteUint32(&info.compression, fp)) return false;
		if (!WriteUint32(&info.size_data, fp)) return false;
		if (!WriteUint32(&info.hres, fp)) return false;
		if (!WriteUint32(&info.vres, fp)) return false;
		if (!WriteUint32(&info.colors, fp)) return false;
		if (!WriteUint32(&info.important_colors, fp)) return false;

		return true;
	}

	static bool ReadPalette(BMP_PALETTE* palette, FILE* fp)
	{
		if (palette == NULL || fp == NULL) return false;

		if (!ReadUint8(&palette->b, fp)) return false;
		if (!ReadUint8(&palette->g, fp)) return false;
		if (!ReadUint8(&palette->r, fp)) return false;
		if (!ReadUint8(&palette->a, fp)) return false;

		return true;
	}

	static bool Decode(uint8_t* data, size_t size, size_t bits)
	{
		switch (bits)
		{
		case 24:
			return bgr2rgb(data, size);
			break;
		case 32:
			return bgra2rgba(data, size);
			break;
		};

		return true;
	}

	bool ClmbsImg_IsBMP(const char* file)
	{
		FILE* fp = fopen(file, "rb");
		if (fp == NULL)
			return false;

		unsigned char magic[2];
		fread(magic, sizeof(magic), 1, fp);
		fclose(fp);

		if (magic[0] == 'B' && magic[1] == 'M')
			return true;
		else
			return false;
	}

	ClmbsImg_Data ClmbsImg_LoadBMP(const char* file)
	{
		ClmbsImg_Data ret;

		FILE* fp = fopen(file, "rb");
		if (fp == NULL) return ret;

		BMP_HEADER header;
		BMP_INFO info;

		if (!ReadHeader(&header, fp)) return ret;
		if (!ReadInfo(&info, fp)) return ret;

		uint8_t* data = malloc(header.size - 68);
		fread(data, header.size - 68, 1, fp);

		Decode(data, header.size - 68, info.bits);

		ret.w = info.width;
		ret.h = info.height;
		ret.bpp = info.bits / 8;

		ret.data = data;

		fclose(fp);

		return ret;
	}

	bool ClmbsImg_SaveBMP(const char* file, ClmbsImg_Data data)
	{
		FILE* fp = fopen(file, "wb");
		if (fp == NULL) return false;

		BMP_HEADER header;
		BMP_INFO info;

		header.magic[0] = 'B';
		header.magic[1] = 'M';
		header.size = data.w * data.h * data.bpp + 54;
		header.unused = 0;
		header.offset = 54;

		info.infosize = 40;
		info.width = data.w;
		info.height = data.h;
		info.planes = 1;
		info.bits = data.bpp * 8;
		info.compression = 0;
		info.size_data = data.w * data.h * data.bpp;
		info.hres = 0;
		info.vres = 0;
		info.colors = 0;
		info.important_colors = 0;

		if (!WriteHeader(header, fp)) return false;
		if (!WriteInfo(info, fp)) return false;

		uint8_t* buffer = (uint8_t*)malloc(data.w * data.h * data.bpp);
		memcpy(buffer, data.data, data.w * data.h * data.bpp);

		Decode(buffer, data.w * data.h * data.bpp, data.bpp * 8);
		
		fwrite(buffer, data.w * data.h * data.bpp, 1, fp);

		fclose(fp);
		free(buffer);

		return true;
	}

#ifdef __cplusplus
}
#endif









