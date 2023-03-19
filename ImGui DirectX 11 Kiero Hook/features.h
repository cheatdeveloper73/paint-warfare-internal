#pragma once

#include "config.h"
#include <cstdint>
#include "il2cpp/il2cpp.h"
#include "il2cpp.h"
#include "globals.h"
#include <algorithm>

namespace features
{

	void infinite_ammo(Weapon_o* current_weapon);
	
	void no_spread(Weapon_o* current_weapon);

	void rapid_fire(Weapon_o* current_weapon);

	void no_recoil(Weapon_o* current_weapon);

}