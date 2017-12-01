#include "ColumbusImage.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

	int ReadBytes(void* data, size_t size, FILE* fp)
	{
		if (fread(data, size, 1, fp) != 1) return 0;
		return 1;
	}

	int ReadUint8(uint8_t* data, FILE* fp)
	{
		return ReadBytes(data, sizeof(uint8_t), fp);
	}

	int ReadUint16(uint16_t* data, FILE* fp)
	{
		return ReadBytes(data, sizeof(uint16_t), fp);
	}

	int ReadUint32(uint32_t* data, FILE* fp)
	{
		return ReadBytes(data, sizeof(uint32_t), fp);
	}

	int ReadInt32(int32_t* data, FILE* fp)
	{
		return ReadBytes(data, sizeof(int32_t), fp);
	}

	typedef struct
	{
		uint8_t magic[2]; //Magic Bytes 'B' and 'M'
		uint32_t size; //Size of whole file
		uint32_t unused; //Should be 0
		uint32_t offset; //Offset to bitmap data
	} BMP_HEADER;

	typedef struct
	{
		uint32_t infosize; //Size of info struct
		int32_t width; //Width of image
		int32_t height; //Height of image
		uint16_t planes; //Should be 1
		uint16_t bits; //Bits per pixel (1, 4, 8, 16, 24, 32)
		uint32_t compression; //0 = none, 1 = 8-bit RLE, 2 = 4-bit RLE
	} BMP_INFO;

	typedef struct
	{
		uint8_t b; //Blue channel
		uint8_t g; //Green channel
		uint8_t r; //Red channel
		uint8_t a; //Alpha channel
	} BMP_PALETTE;

	bool ReadHeader(BMP_HEADER* header, FILE* fp)
	{
		if (header == NULL || fp == NULL) return false;

		if (!ReadUint8(&header->magic[0], fp)) return false;
		if (!ReadUint8(&header->magic[1], fp)) return false;
		if (!ReadUint32(&header->size, fp)) return false;
		if (!ReadUint32(&header->unused, fp)) return false;
		if (!ReadUint32(&header->offset, fp)) return false;

		return true;
	}

	bool ReadInfo(BMP_INFO* info, FILE* fp)
	{
		if (info == NULL || fp == NULL) return false;

		if (!ReadUint32(&info->infosize, fp)) return false;
		if (!ReadInt32(&info->width, fp)) return false;
		if (!ReadInt32(&info->height, fp)) return false;
		if (!ReadUint16(&info->planes, fp)) return false;
		if (!ReadUint16(&info->bits, fp)) return false;
		if (!ReadUint32(&info->compression, fp)) return false;

		return true;
	}

	bool ReadPalette(BMP_PALETTE* palette, FILE* fp)
	{
		if (palette == NULL || fp == NULL) return false;

		if (!ReadUint8(&palette->b, fp)) return false;
		if (!ReadUint8(&palette->g, fp)) return false;
		if (!ReadUint8(&palette->r, fp)) return false;
		if (!ReadUint8(&palette->a, fp)) return false;

		return true;
	}

	bool Decode24(uint8_t* data, size_t size)
	{
		for (int i = 0; i < size; i += 3)
		{
			data[i + 0] = data[i + 2];
			data[i + 1] = data[i + 0];
			data[i + 2] = data[i - 2];
		}

		return true;
	}

	bool Decode(uint8_t* data, size_t size, size_t bits)
	{
		switch (bits)
		{
		case 24:
			return Decode24(data, size);
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

		ret.w = info.width;
		ret.h = info.height;
		ret.bpp = info.bits / 8;

		uint8_t* data = malloc(header.size - 122);
		fread(data, header.size - 122, 1, fp);

		Decode(data, header.size - 122, info.bits);

		fclose(fp);

		return ret;
	}

#ifdef __cplusplus
}
#endif








