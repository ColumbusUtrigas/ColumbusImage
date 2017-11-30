#pragma once

namespace Columbus
{

	struct ImageData
	{
		unsigned int w; //Image width
		unsigned int h; //Image height
		unsigned int bpp; //Bytes per pixel: 3, 4
		unsigned char* data; //Image pixel data
	};

}







