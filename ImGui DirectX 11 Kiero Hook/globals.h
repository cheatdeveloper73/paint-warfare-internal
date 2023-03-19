#pragma once

#include "il2cpp.h"

#include <mutex>
#include <vector>

namespace globals
{

	inline std::mutex player_sync{};

}