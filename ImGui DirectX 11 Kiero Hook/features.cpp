#include "features.h"

void features::infinite_ammo(Weapon_o* current_weapon)
{

	current_weapon->fields.ammo = 2u;

}

void features::no_spread(Weapon_o* current_weapon)
{

	current_weapon->fields.spread = 0.f;

}

void features::rapid_fire(Weapon_o* current_weapon)
{

	current_weapon->fields.nextFire = 0.f;

}

void features::no_recoil(Weapon_o* current_weapon)
{

	current_weapon->fields.recoilLength = false;
	current_weapon->fields.recoilXY.fields.x = 0.f;
	current_weapon->fields.recoilXY.fields.y = 0.f;

}