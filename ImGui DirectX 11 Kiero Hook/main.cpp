#include "includes.h"
#include "hooks/hooks.h"
#include "il2cpp/il2cpp.h"
#include "globals.h"
#include "features.h"
#include "menu/menu.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			menu.Initialize();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (GetAsyncKeyState(VK_INSERT) & 1)
		menu.opened = !menu.opened;

	menu.Render();

	static auto player_spawner_class = il2cpp::find_class("", "PlayerSpawner");
	auto player_spawner = il2cpp::get_static_field_value<PlayerSpawner_o*>(player_spawner_class, "instance");

	static auto weapon_class = il2cpp::find_class("", "Weapon");
	auto current_weapon = il2cpp::get_static_field_value<Weapon_o*>(weapon_class, "currentWeapon");

	static auto movement_class = il2cpp::find_class("", "Movement");
	auto player_movement = il2cpp::get_static_field_value<Movement_o*>(movement_class, "instance");

	if (player_spawner)
	{

		if (auto local_player = player_spawner->fields.localPlayerSetup && current_weapon)
		{

			if (config::infinite_ammo) features::infinite_ammo(current_weapon);

			if (config::no_recoil) features::no_recoil(current_weapon);

			if (config::no_spread) features::no_spread(current_weapon);

			if (config::rapid_fire) features::rapid_fire(current_weapon);

		}

	}

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			AllocConsole();
			SetConsoleTitleA("redhook (rellant.dev)");
			FILE* file;
			freopen_s(&file, "CONOUT$", "w", stdout);
			il2cpp::sanity();
			hooks::init();
			kiero::bind(8, (void**)& oPresent, hkPresent);
			std::cout << "initialized\n";
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}