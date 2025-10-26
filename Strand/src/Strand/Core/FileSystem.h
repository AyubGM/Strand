#pragma once

#include "Strand/Core/Buffer.h"
#include <filesystem>

namespace Strand {

	class FileSystem
	{
	public:
		// TODO: move to FileSystem class
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};

}