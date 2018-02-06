#include "ClmbsImg_Util.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

	int ReadBytes(void* data, size_t size, FILE* fp)
	{
		if (fread(data, size, 1, fp) != 1) return 0;
		return 1;
	}

	int WriteBytes(const void* data, size_t size, FILE* fp)
	{
		if (fwrite(data, size, 1, fp) != 1) return 0;
		return 1;
	}

	int ReadUint8(uint8_t* data, FILE* fp)
	{
		return ReadBytes(data, sizeof(uint8_t), fp);
	}

	int WriteUint8(const uint8_t* data, FILE* fp)
	{
		return WriteBytes(data, sizeof(uint8_t), fp);
	}

	int ReadUint16(uint16_t* data, FILE* fp)
	{
		return ReadBytes(data, sizeof(uint16_t), fp);
	}

	int WriteUint16(const uint16_t* data, FILE* fp)
	{
		return WriteBytes(data, sizeof(uint16_t), fp);
	}

	int ReadUint32(uint32_t* data, FILE* fp)
	{
		return ReadBytes(data, sizeof(uint32_t), fp);
	}

	int WriteUint32(const uint32_t* data, FILE* fp)
	{
		return WriteBytes(data, sizeof(uint32_t), fp);
	}

	int ReadInt32(int32_t* data, FILE* fp)
	{
		return ReadBytes(data, sizeof(int32_t), fp);
	}

	int WriteInt32(const int32_t* data, FILE* fp)
	{
		return WriteBytes(data, sizeof(int32_t), fp);
	}

	int bgr2rgb(uint8_t* data, size_t size)
	{
		if (data == NULL) return 0;

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

		return 1;
	}

	int bgra2rgba(uint8_t* data, size_t size)
	{
		if (data == NULL) return 0;

		uint8_t bgr[3];
		for (int i = 0; i < size; i += 4)
		{
			bgr[0] = data[i + 0];
			bgr[1] = data[i + 1];
			bgr[2] = data[i + 2];

			data[i + 0] = bgr[2];
			data[i + 1] = bgr[1];
			data[i + 2] = bgr[0];
			data[i + 3] = data[i + 3];
		}

		return 1;
	}

	int rgb2bgr(uint8_t* data, size_t size)
	{
		return bgr2rgb(data, size);
	}

	int rgba2bgra(uint8_t* data, size_t size)
	{
		return bgra2rgba(data, size);
	}

	int flipX(uint8_t* aData, size_t aWidth, size_t aHeight, size_t aBPP)
	{
		if (aData == NULL) return 0;

		const size_t stride = aWidth * aBPP;
		uint8_t* row;
		uint8_t* pixel = (uint8_t*)malloc(aBPP);

		for (size_t i = 0; i < aHeight; i++)
		{
			row = &aData[i * stride];

			for (size_t j = 0; j < aWidth / 2; j++)
			{
				memcpy(pixel, &row[j * aBPP], aBPP);
				memcpy(&row[j * aBPP], &row[(aWidth - j) * aBPP], aBPP);
				memcpy(&row[(aWidth - j) * aBPP], pixel, aBPP);
			}
		}

		return 1;
	}

	int flipY(uint8_t* aData, size_t aWidth, size_t aHeight, size_t aBPP)
	{
		if (aData == NULL) return 0;

		const size_t stride = aWidth * aBPP;
		uint8_t* row = (uint8_t*)malloc(stride);

		for (size_t i = 0; i < aHeight / 2; i++)
		{
			memcpy(row, &aData[i * stride], stride);
			memcpy(&aData[i * stride], &aData[(aHeight - i) * stride], stride);
			memcpy(&aData[(aHeight - i) * stride], row, stride);
		}
		free(row);

		return 1;
	}

	int flipXY(uint8_t* aData, size_t aWidth, size_t aHeight, size_t aBPP)
	{
		if (aData == NULL) return 0;

		const size_t size = aWidth * aHeight;
		//uint8_t* pixel = new uint8_t[aBPP];
		uint8_t* pixel = (uint8_t*)malloc(sizeof(uint8_t) * aBPP);

		for (size_t i = 0; i < size / 2; i++)
		{
			memcpy(pixel, &aData[i * aBPP], aBPP);
			memcpy(&aData[i * aBPP], &aData[(size - i) * aBPP], aBPP);
			memcpy(&aData[(size - i) * aBPP], pixel, aBPP);
		}
		free(pixel);

		return 1;
	}

#ifdef __cplusplus
}
#endif





