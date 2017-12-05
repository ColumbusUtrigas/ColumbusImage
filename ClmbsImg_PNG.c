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

	bool ClmbsImg_IsPNG(const char* file)
	{
		FILE* fp = fopen(file, "rb");
		if (fp == NULL) return false;

		uint8_t magic[4];
		if (!ReadBytes(magic, sizeof(magic), fp)) return false;
		fclose(fp);

		if (magic[1] == 'P' &&
			magic[2] == 'N' &&
			magic[3] == 'G') return true;
		else return false;
	}

#ifdef __cplusplus
}
#endif




