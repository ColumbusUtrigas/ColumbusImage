#include "ColumbusImage.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
#endif

	void ClmbsImg_Free(ClmbsImg_Data* data)
	{
		if (data == NULL) return;

		data->w = 0;
		data->h = 0;
		data->bpp = 0;

		if (data->data == NULL) return;
		free(data->data);
	}

	ClmbsImg_Data ClmbsImg_Load(const char* file)
	{
		ClmbsImg_Data err;

		if (ClmbsImg_IsBMP(file))
			return ClmbsImg_LoadBMP(file);

		if (ClmbsImg_IsPNG(file))
			return ClmbsImg_LoadPNG(file);

		if (ClmbsImg_IsJPG(file))
			return ClmbsImg_LoadJPG(file);

		if (ClmbsImg_IsTIF(file))
			return ClmbsImg_LoadTIF(file);

		if (ClmbsImg_IsTGA(file))
			return ClmbsImg_LoadTGA(file);

		return err;
	}

#ifdef __cplusplus
}
#endif







