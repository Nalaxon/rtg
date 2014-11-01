


#ifndef INCLUDED_FRAMEWORK_PNG_FILE_FORMAT
#define INCLUDED_FRAMEWORK_PNG_FILE_FORMAT

#pragma once

#include <cstdint>

#include "image.h"


namespace PNG
{
	image<std::uint32_t> loadImage2D(const char* filename);
	void saveImage(const char* filename, const image<std::uint32_t>& surface);
}

#endif  // INCLUDED_FRAMEWORK_PNG_FILE_FORMAT
