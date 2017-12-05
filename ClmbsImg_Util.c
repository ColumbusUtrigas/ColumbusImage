#include "ClmbsImg_Util.h"
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

#ifdef __cplusplus
}
#endif





