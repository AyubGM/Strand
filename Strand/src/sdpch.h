#pragma once

#include "Strand/Core/PlatformDetection.h"

#ifdef SD_PLATFORM_WINDOWS
#ifndef NOMINMAX
// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
#define NOMINMAX
#endif
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Strand/Core/Base.h"
#include "Strand/Core/Buffer.h"

#include "Strand/Core/Log.h"

#include "Strand/Debug/Instrumentor.h"

#ifdef SD_PLATFORM_WINDOWS
	#include <Windows.h>
#endif // SD_PLATFORM_WINDOWS
