#include "hooks.h"

#include "../globals.h"
#include "../features.h"

bool hooks::hook_function(void* function_pointer, void* callback_function)
{

	if (!function_pointer || !callback_function)
		return false;

	hooked_function fn;
	fn.callback_function = callback_function;
	fn.target_address = function_pointer;

	MH_CreateHook((uintptr_t*)function_pointer, callback_function, (LPVOID*)&fn.original_function);

	hooks.push_back(fn);

	return MH_EnableHook(function_pointer) == MH_OK;

}

void hk_anticheat_manager_fixed_update(AntiCheatManager_o* thisptr)
{
	return; // bypa$$a
}

void hk_player_setup_anticheat(PlayerSetup_o* thisptr)
{
	return; // bypa$$a
}

void hk_save_system_check_save(SaveSystem_o* thisptr)
{
	return;
}

void hk_player_movement_update(PlayerMovement_o* thisptr)
{

	reinterpret_cast<decltype(&hk_player_movement_update)>(hooks::hooks[3].original_function)(thisptr);

	thisptr->fields.jumpHeight = config::jump_height;
	thisptr->fields.baseSpeed = config::player_speed;
	thisptr->fields.gravity = config::player_gravity;

}

void hooks::init()
{

	if (!hook_function(il2cpp::get_method("", "AntiCheatManager", "FixedUpdate")->methodPointer, hk_anticheat_manager_fixed_update))
		exit(0);

	if (!hook_function(il2cpp::get_method("", "PlayerSetup", "AntiCheat")->methodPointer, hk_player_setup_anticheat))
		exit(0);

	if (!hook_function(il2cpp::get_method("", "SaveSystem", "CheckSaveFile")->methodPointer, hk_player_setup_anticheat))
		exit(0);

	if (!hook_function(il2cpp::get_method("", "PlayerMovement", "Update")->methodPointer, hk_player_setup_anticheat))
		exit(0);

}