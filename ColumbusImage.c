#include "ColumbusImage.h"

#ifdef __cplusplus
extern "C"
#endif

	ClmbsImg_Data ClmbsImg_Load(const char* file)
	{
		ClmbsImg_Data err;

		if (ClmbsImg_IsBMP(file))
			return ClmbsImg_LoadBMP(file);

		return err;
	}

#ifdef __cplusplus
}
#endif







