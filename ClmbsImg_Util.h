#include "ColumbusImage.h"
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

	int ReadBytes(void* data, size_t size, FILE* fp);

	int WriteBytes(const void* data, size_t size, FILE* fp);

	int ReadUint8(uint8_t* data, FILE* fp);

	int WriteUint8(const uint8_t* data, FILE* fp);

	int ReadUint16(uint16_t* data, FILE* fp);

	int WriteUint16(const uint16_t* data, FILE* fp);

	int ReadUint32(uint32_t* data, FILE* fp);

	int WriteUint32(const uint32_t* data, FILE* fp);

	int ReadInt32(int32_t* data, FILE* fp);

	int WriteInt32(const int32_t* data, FILE* fp);

#ifdef __cplusplus
}
#endif



