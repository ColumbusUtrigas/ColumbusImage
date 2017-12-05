#include "ColumbusImage.h"

#ifdef __cplusplus
extern "C"
#endif

	ClmbsImg_Data ClmbsImg_Load(const char* file)
	{
		ClmbsImg_Data err;

		if (ClmbsImg_IsBMP(file))
			return ClmbsImg_LoadBMP(file);

		if (ClmbsImg_IsPNG(file))
			return ClmbsImg_LoadPNG(file);

		if (ClmbsImg_IsTGA(file))
			return ClmbsImg_LoadTGA(file);

		return err;
	}

#ifdef __cplusplus
}
#endif







