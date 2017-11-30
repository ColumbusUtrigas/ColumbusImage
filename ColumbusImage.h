#pragma once

#include "ImageTypes.h"
#include "ImageBMP.h"

namespace Columbus
{

	ImageData LoadImage(const char* file);


	ImageData LoadImage(const char* file)
	{
		ImageData err;

		if (IsBMP(file))
			return LoadBMP(file);

		return err;
	}

}







