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

	int bgr2rgb(uint8_t* data, size_t size);
	int bgra2rgba(uint8_t* data, size_t size);
	int rgb2bgr(uint8_t* data, size_t size);
	int rgba2bgra(uint8_t* data, size_t size);

	int flipX(uint8_t* aData, size_t aWidth, size_t aHeight, size_t aBPP);
	int flipY(uint8_t* aData, size_t aWidth, size_t aHeight, size_t aBPP);
	int flipXY(uint8_t* aData, size_t aWidth, size_t aHeight, size_t aBPP);

#ifdef __cplusplus
}
#endif



