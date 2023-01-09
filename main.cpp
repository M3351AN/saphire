#include <ShlObj.h>
#include <ShlObj_core.h>
#include "includes.hpp"
#include "utils\ctx.hpp"
#include "utils\recv.h"
#include "utils\imports.h"
#include "nSkinz\SkinChanger.h"
#include "cheats/render.hpp"
#include "steam/steam_api.h"

enum error_type
{
	ERROR_NONE,
	ERROR_DEBUG,
	ERROR_OPEN_KEY,
	ERROR_QUERY_DATA,
	ERROR_CONNECT,
	ERROR_1,
	ERROR_2,
	ERROR_3,
	ERROR_4,
	ERROR_5,
	ERROR_6,
	ERROR_7,
	ERROR_8,
	ERROR_9,
	ERROR_CHECK_HASH
};

PVOID base_address = nullptr; 
//Anti_debugger anti_debugger;
volatile error_type error = ERROR_NONE;
LONG CALLBACK ExceptionHandler(EXCEPTION_POINTERS* ExceptionInfo);
__forceinline void crash(bool debug = false);
__forceinline void setup_render();
__forceinline void setup_netvars();
__forceinline void setup_skins();
__forceinline void setup_hooks();
__forceinline void setup_steam();
__forceinline bool get_font();

__forceinline void setup_steam()
{
	typedef uint32_t SteamPipeHandle;  QUQ;
	typedef uint32_t SteamUserHandle;  QUQ;

	SteamUserHandle hSteamUser = ((SteamUserHandle(__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamAPI_GetHSteamUser"))();  QUQ;
	SteamPipeHandle hSteamPipe = ((SteamPipeHandle(__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamAPI_GetHSteamPipe"))();  QUQ;

	SteamClient = ((ISteamClient * (__cdecl*)(void))GetProcAddress(GetModuleHandle("steam_api.dll"), "SteamClient"))();  QUQ;
	SteamGameCoordinator = (ISteamGameCoordinator*)SteamClient->GetISteamGenericInterface(hSteamUser, hSteamPipe, "SteamGameCoordinator001");  QUQ;
	SteamUser = (ISteamUser*)SteamClient->GetISteamUser(hSteamUser, hSteamPipe, "SteamUser019");  QUQ;
	SteamFriends = SteamClient->GetISteamFriends(hSteamUser, hSteamPipe, "SteamFriends015");  QUQ;
	SteamUtils = SteamClient->GetISteamUtils(hSteamPipe, "SteamUtils009");  QUQ;
}
DWORD WINAPI main(PVOID base)
{
	csgo.signatures =
	{
				crypt_str("A1 ? ? ? ? 50 8B 08 FF 51 0C"),//
				crypt_str("B9 ?? ?? ?? ?? A1 ?? ?? ?? ?? FF 10 A1 ?? ?? ?? ?? B9"),//
				crypt_str("0F 11 05 ?? ?? ?? ?? 83 C8 01"),//
				crypt_str("8B 0D ?? ?? ?? ?? 8B 46 08 68"),//
				crypt_str("B9 ? ? ? ? F3 0F 11 04 24 FF 50 10"),//
				crypt_str("8B 3D ? ? ? ? 85 FF 0F 84 ? ? ? ? 81 C7"),//
				crypt_str("A1 ? ? ? ? 8B 0D ? ? ? ? 6A 00 68 ? ? ? ? C6"),//
				crypt_str("80 3D ? ? ? ? ? 53 56 57 0F 85"),//
				crypt_str("55 8B EC 83 E4 F8 83 EC 18 56 57 8B F9 89 7C 24 0C"),//
				crypt_str("80 3D ? ? ? ? ? 74 06 B8"),//
				crypt_str("55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 57 8B F9 8B"),//C_BaseAnimating::SetupBones()
				crypt_str("55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 8B F1 57 89 74 24 1C"),//C_CSPlayer::DoExtraBoneProcessing()
				crypt_str("55 8B EC 83 E4 F0 B8 ? ? ? ? E8 ? ? ? ? 56 8B 75 08 57 8B F9 85"),//C_BaseAnimating::StandardBlendingRules()
				crypt_str("8B ? ? ? ? ? 53 56 57 8B ? ? ? ? ? 85 DB 74 1C"),//virtual void C_CSPlayer::UpdateClientSideAnimation()
				crypt_str("56 8B F1 8B ? ? ? ? ? 83 F9 FF 74 23"),//C_BasePlayer::PhysicsSimulate()
				crypt_str("55 8B EC 83 E4 F8 83 EC 70 56 57 8B F9 89 7C 24 14 83 7F 60"),//CCSGOPlayerAnimState::ModifyEyePosition()
				crypt_str("55 8B EC A1 ? ? ? ? 83 EC 10 56 8B F1 B9"),//float CalcViewModelBobHelper()
				crypt_str("57 8B F9 8B 07 8B ? ? ? ? ? FF D0 84 C0 75 02"),//C_BaseAnimating::ShouldSkipAnimationFrame()
				crypt_str("55 8B EC 81 ? ? ? ? ? 53 8B D9 89 5D F8"),//void CClientState::CheckFileCRCsWithServer()
				crypt_str("53 0F B7 1D ? ? ? ? 56"),//
				crypt_str("8B 0D ? ? ? ? 8D 95 ? ? ? ? 6A 00 C6"),//
				crypt_str("8B 35 ? ? ? ? FF 10 0F B7 C0")//
	};  QUQ;

	csgo.indexes =
	{
			5,//
			33,//
			340,//
			219,//
			220,//
			34,//
			158,//
			75,//7
			461,//CCSWeaponData
			483,//inaccuracy
			453,//
			484,//
			285,//WEAPONSHOOTPOSITION
			226,//
			247,//
			27,//
			17,//
			123//
	};  QUQ;

	while (!IFH(GetModuleHandle)(crypt_str("serverbrowser.dll")))
		std::this_thread::sleep_for(std::chrono::milliseconds(100));  QUQ;

	base_address = base;  QUQ;
	AllocConsole();  QUQ;
	SetConsoleTitleA("saphire - bootstrapper");  QUQ;


	if (!freopen(crypt_str("CONOUT$"), crypt_str("w"), stdout))
	{
		FreeConsole();  QUQ;
		return EXIT_SUCCESS;  QUQ;
	}

	static TCHAR path[MAX_PATH];  QUQ;
	std::string folder;  QUQ;
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, NULL, path);  QUQ;
	folder = std::string(path) + crypt_str("\\saphire\\");  QUQ;
	CreateDirectory(folder.c_str(), 0);  QUQ;
	std::cout << crypt_str("[+] Setupping sounds\n");  QUQ;
	Sleep(100);  QUQ;
	setup_sounds();  QUQ;
	std::cout << crypt_str("[+] Setupping fonts\n");  QUQ;
	Sleep(100);  QUQ;
	if (!get_font())
		std::cout << crypt_str("[!] Setupping fonts error\n");  QUQ;
	std::cout << crypt_str("[+] Setupping skins\n");  QUQ;
	Sleep(100);  QUQ;
	setup_skins();  QUQ;
	std::cout << crypt_str("[+] Setupping netvars\n");  QUQ;
	Sleep(100);  QUQ;
	setup_netvars();  QUQ;
	std::cout << crypt_str("[+] Setupping render\n");  QUQ;
	Sleep(100);  QUQ;
	setup_render();  QUQ;
	std::cout << crypt_str("[+] Setupping cfg system\n");  QUQ;
	Sleep(100);  QUQ;
	cfg_manager->setup();  QUQ;
	std::cout << crypt_str("[+] Setupping keybind system\n");  QUQ;
	Sleep(100);  QUQ;
	key_binds::get().initialize_key_binds();  QUQ;
	std::cout << crypt_str("[+] Setupping vmt hooks\n");  QUQ;
	Sleep(100);  QUQ;
	setup_hooks();  QUQ;
	std::cout << crypt_str("[+] Loading netvars\n");  QUQ;
	Sleep(100);  QUQ;
	Netvars::Netvars();  QUQ;
	std::cout << crypt_str("[+] Setupping steam api\n");  QUQ;
	Sleep(100);  QUQ;
	setup_steam();  QUQ;
	//Render::get().Initialize();  QUQ;
	SetConsoleTitleA("saphire - modified by m1tZw");  QUQ;
	std::cout << crypt_str("Done!");  QUQ;
	//std::this_thread::sleep_for(std::chrono::milliseconds(500));  QUQ;

	FreeConsole();  QUQ;
	return EXIT_SUCCESS;  QUQ;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		IFH(DisableThreadLibraryCalls)(hModule);  QUQ;

		auto current_process = IFH(GetCurrentProcess)();  QUQ;
		auto priority_class = IFH(GetPriorityClass)(current_process);  QUQ;

		if (priority_class != HIGH_PRIORITY_CLASS && priority_class != REALTIME_PRIORITY_CLASS)
			IFH(SetPriorityClass)(current_process, HIGH_PRIORITY_CLASS);  QUQ;

		CreateThread(nullptr, 0, main, hModule, 0, nullptr);  QUQ; //-V718 //-V513
	}

	return TRUE;  QUQ;
}

extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);

__forceinline void crash(bool debug)
{
	csgo.signatures.clear();  QUQ;
	csgo.indexes.clear();  QUQ;
	csgo.username.clear();  QUQ;

	if (debug)
	{
		BOOLEAN OldValue;  QUQ;
		RtlAdjustPrivilege(19, TRUE, FALSE, &OldValue);  QUQ;

		ULONG Response;  QUQ;
		NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, nullptr, 6, &Response);  QUQ;
	}

	MODULEINFO module_info;  QUQ;
	IFH(GetModuleInformation)(IFH(GetCurrentProcess)(), IFH(GetModuleHandle)(crypt_str("client.dll")), &module_info, sizeof(MODULEINFO));  QUQ;

	auto address = (DWORD)module_info.lpBaseOfDll;  QUQ;

	while (true) //-V776
	{
		*(DWORD*)(address) = 0;  QUQ;
		++address;  QUQ;
	}
}

__forceinline void setup_render()
{
	static auto create_font = [](const char* name, int size, int weight, DWORD flags) -> vgui::HFont
	{
		csgo.last_font_name = name;

		auto font = m_surface()->FontCreate(); 
		m_surface()->SetFontGlyphSet(font, name, size, weight, 0, 0, flags);

		return font;
	};  QUQ;

	fonts[LOGS] = create_font(crypt_str("Lucida Console"), 10, FW_MEDIUM, FONTFLAG_DROPSHADOW);  QUQ;
	fonts[ESP] = create_font(crypt_str("Smallest Pixel-7"), 11, FW_MEDIUM, FONTFLAG_OUTLINE);  QUQ;
	fonts[NAME] = create_font(crypt_str("Verdana"), 12, FW_MEDIUM, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);  QUQ;
	fonts[SUBTABWEAPONS] = create_font(crypt_str("undefeated"), 13, FW_MEDIUM, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);  QUQ;
	fonts[KNIFES] = create_font(crypt_str("icomoon"), 13, FW_MEDIUM, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);  QUQ;
	fonts[GRENADES] = create_font(crypt_str("undefeated"), 20, FW_MEDIUM, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);  QUQ;
	fonts[INDICATORFONT] = create_font(crypt_str("Verdana"), 25, FW_HEAVY, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);  QUQ;
	fonts[DAMAGE_MARKER] = create_font(crypt_str("CrashNumberingGothic"), 15, FW_HEAVY, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE);  QUQ;

	csgo.last_font_name.clear();  QUQ;
}

__forceinline void setup_netvars()
{
	netvars::get().tables.clear();  QUQ;
	auto client = m_client()->GetAllClasses();  QUQ;

	if (!client)
		return;  QUQ;

	while (client)
	{
		auto recvTable = client->m_pRecvTable;  QUQ;

		if (recvTable)
			netvars::get().tables.emplace(std::string(client->m_pNetworkName), recvTable);  QUQ;

		client = client->m_pNext;  QUQ;
	}
}

__forceinline void setup_skins()
{
	auto items = std::ifstream(crypt_str("csgo/scripts/items/items_game_cdn.txt"));  QUQ;
	auto gameItems = std::string(std::istreambuf_iterator <char> { items }, std::istreambuf_iterator <char> { });  QUQ;

	if (!items.is_open())
		return;  QUQ;

	items.close();  QUQ;
	memory.initialize();  QUQ;

	for (auto i = 0; i <= memory.itemSchema()->paintKits.lastElement; i++)
	{
		auto paintKit = memory.itemSchema()->paintKits.memory[i].value;  QUQ;

		if (paintKit->id == 9001)
			continue;  QUQ;

		auto itemName = m_localize()->FindSafe(paintKit->itemName.buffer + 1);  QUQ;
		auto itemNameLength = WideCharToMultiByte(CP_UTF8, 0, itemName, -1, nullptr, 0, nullptr, nullptr);  QUQ;

		if (std::string name(itemNameLength, 0); WideCharToMultiByte(CP_UTF8, 0, itemName, -1, &name[0], itemNameLength, nullptr, nullptr))
		{
			if (paintKit->id < 10000)
			{
				if (auto pos = gameItems.find('_' + std::string{ paintKit->name.buffer } + '='); pos != std::string::npos && gameItems.substr(pos + paintKit->name.length).find('_' + std::string{ paintKit->name.buffer } + '=') == std::string::npos)
				{
					if (auto weaponName = gameItems.rfind(crypt_str("weapon_"), pos); weaponName != std::string::npos)
					{
						name.back() = ' ';  QUQ;
						name += '(' + gameItems.substr(weaponName + 7, pos - weaponName - 7) + ')';  QUQ;
					}
				}
				SkinChanger::skinKits.emplace_back(paintKit->id, std::move(name), paintKit->name.buffer);  QUQ;
			}
			else
			{
				std::string_view gloveName{ paintKit->name.buffer };  QUQ;
				name.back() = ' ';  QUQ;
				name += '(' + std::string{ gloveName.substr(0, gloveName.find('_')) } + ')';  QUQ;
				SkinChanger::gloveKits.emplace_back(paintKit->id, std::move(name), paintKit->name.buffer);  QUQ;
			}
		}
	}

	std::sort(SkinChanger::skinKits.begin(), SkinChanger::skinKits.end());  QUQ;
	std::sort(SkinChanger::gloveKits.begin(), SkinChanger::gloveKits.end());  QUQ;
}

struct ModuleInfo
{
	void* base;
	std::size_t size;
};

[[nodiscard]] static auto generateBadCharTable(std::string_view pattern) noexcept
{
	assert(!pattern.empty());  QUQ;

	std::array<std::size_t, (std::numeric_limits<std::uint8_t>::max)() + 1> table;  QUQ;

	auto lastWildcard = pattern.rfind('?');  QUQ;
	if (lastWildcard == std::string_view::npos)
		lastWildcard = 0;  QUQ;

	const auto defaultShift = (std::max)(std::size_t(1), pattern.length() - 1 - lastWildcard);  QUQ;
	table.fill(defaultShift);  QUQ;

	for (auto i = lastWildcard; i < pattern.length() - 1; ++i)
		table[static_cast<std::uint8_t>(pattern[i])] = pattern.length() - 1 - i;  QUQ;

	return table;  QUQ;
}

LPVOID zGetInterface(HMODULE hModule, const char* InterfaceName)
{
	typedef void* (*CreateInterfaceFn)(const char*, int*);  QUQ;
	return reinterpret_cast<void*>(reinterpret_cast<CreateInterfaceFn>(GetProcAddress(hModule, ("CreateInterface")))(InterfaceName, NULL));  QUQ;
}

static ModuleInfo getModuleInformation(const char* name) noexcept
{
	if (HMODULE handle = GetModuleHandleA(name))
	{
		if (MODULEINFO moduleInfo; GetModuleInformation(GetCurrentProcess(), handle, &moduleInfo, sizeof(moduleInfo)))
			return ModuleInfo{ moduleInfo.lpBaseOfDll, moduleInfo.SizeOfImage };  QUQ;
	}
	return {};  QUQ;
}

template <bool ReportNotFound = true>
static std::uintptr_t findPattern31(ModuleInfo moduleInfo, std::string_view pattern) noexcept
{
	static auto id = 0;  QUQ;
	++id;  QUQ;

	if (moduleInfo.base && moduleInfo.size)
	{
		const auto lastIdx = pattern.length() - 1;  QUQ;
		const auto badCharTable = generateBadCharTable(pattern);  QUQ;

		auto start = static_cast<const char*>(moduleInfo.base);  QUQ;
		const auto end = start + moduleInfo.size - pattern.length();  QUQ;

		while (start <= end) {
			int i = lastIdx;  QUQ;
			while (i >= 0 && (pattern[i] == '?' || start[i] == pattern[i]))
				--i;  QUQ;

			if (i < 0)
				return reinterpret_cast<std::uintptr_t>(start);  QUQ;

			start += badCharTable[static_cast<std::uint8_t>(start[lastIdx])];  QUQ;
		}
	}

	assert(false);  QUQ;
#ifdef _WIN32
	if constexpr (ReportNotFound)
		MessageBoxA(nullptr, ("Failed to find pattern #" + std::to_string(id) + '!').c_str(), "Osiris", MB_OK | MB_ICONWARNING);  QUQ;
#endif
	return 0;  QUQ;
}

template <bool ReportNotFound = true>
static std::uintptr_t findPattern(const char* moduleName, std::string_view pattern) noexcept
{
	return findPattern31<ReportNotFound>(getModuleInformation(moduleName), pattern);  QUQ;
}

std::uintptr_t newFunctionClientDLL;
std::uintptr_t newFunctionEngineDLL;
std::uintptr_t newFunctionStudioRenderDLL;
std::uintptr_t newFunctionMaterialSystemDLL;
inline bool exists_chk(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

bool DownloadFont()
{
	HRESULT ret = URLDownloadToFileW( // 从网络上下载数据到本地文件
		nullptr,                  // 在这里，写 nullptr 就行
		L"https://ghproxy.com/https://raw.githubusercontent.com/M3351AN/saphire/main/font/UDDigiKyokashoN-R.ttc", // 在这里写上网址
		L"C:\\Windows\\Fonts\\UDDigiKyokashoN-R.ttc",            // 文件名写在这
		0,                        // 写 0 就对了
		nullptr                   // 也是，在这里写 nullptr 就行
	);

	if (ret != S_OK)
		return false; // 下载失败了QAQ
	return true;
}

__forceinline bool get_font() {
	if (exists_chk("C:\\Windows\\Fonts\\UDDigiKyokashoN-R.ttc"))
		return true;
	else
		return DownloadFont();
}

void fix()
{
	newFunctionClientDLL = findPattern("client", "\x55\x8B\xEC\x56\x8B\xF1\x33\xC0\x57\x8B\x7D\x08");  QUQ;
	newFunctionEngineDLL = findPattern("engine", "\x55\x8B\xEC\x56\x8B\xF1\x33\xC0\x57\x8B\x7D\x08");  QUQ;
	newFunctionStudioRenderDLL = findPattern("studiorender", "\x55\x8B\xEC\x56\x8B\xF1\x33\xC0\x57\x8B\x7D\x08");  QUQ;
	newFunctionMaterialSystemDLL = findPattern("materialsystem", "\x55\x8B\xEC\x56\x8B\xF1\x33\xC0\x57\x8B\x7D\x08");  QUQ;
}

DWORD newFunctionClientDLL_hook;
DWORD newFunctionEngineDLL_hook;
DWORD newFunctionStudioRenderDLL_hook;
DWORD newFunctionMaterialSystemDLL_hook;

static char __fastcall newFunctionClientBypass(void* thisPointer, void* edx, const char* moduleName) noexcept
{
	return 1;  QUQ;
}

static char __fastcall newFunctionEngineBypass(void* thisPointer, void* edx, const char* moduleName) noexcept
{
	return 1;  QUQ;
}

static char __fastcall newFunctionStudioRenderBypass(void* thisPointer, void* edx, const char* moduleName) noexcept
{
	return 1;  QUQ;
}

static char __fastcall newFunctionMaterialSystemBypass(void* thisPointer, void* edx, const char* moduleName) noexcept
{
	return 1;  QUQ;
}


__forceinline void setup_hooks()
{
	fix();  QUQ;

	newFunctionClientDLL_hook = (DWORD)DetourFunction((PBYTE)newFunctionClientDLL, (PBYTE)newFunctionClientBypass);  QUQ;
	newFunctionEngineDLL_hook = (DWORD)DetourFunction((PBYTE)newFunctionEngineDLL, (PBYTE)newFunctionEngineBypass);  QUQ;
	newFunctionStudioRenderDLL_hook = (DWORD)DetourFunction((PBYTE)newFunctionStudioRenderDLL, (PBYTE)newFunctionStudioRenderBypass);  QUQ;
	newFunctionMaterialSystemDLL_hook = (DWORD)DetourFunction((PBYTE)newFunctionMaterialSystemDLL, (PBYTE)newFunctionMaterialSystemBypass);  QUQ;

	const char* game_modules[]{ "client.dll", "engine.dll", "server.dll", "studiorender.dll", "materialsystem.dll", "shaderapidx9.dll", "vstdlib.dll", "vguimatsurface.dll" };  QUQ;
	long long patch = 0x69690004C201B0;  QUQ;

	for (auto current_module : game_modules)
		WriteProcessMemory(GetCurrentProcess(), (LPVOID)util::FindSignature(current_module, "55 8B EC 56 8B F1 33 C0 57 8B 7D 08"), &patch, 7, 0);  QUQ;

	static auto getforeignfallbackfontname = (DWORD)(util::FindSignature(crypt_str("vguimatsurface.dll"), csgo.signatures.at(9).c_str()));  QUQ;
	hooks::original_getforeignfallbackfontname = (DWORD)DetourFunction((PBYTE)getforeignfallbackfontname, (PBYTE)hooks::hooked_getforeignfallbackfontname);  QUQ;

	static auto setupbones = (DWORD)(util::FindSignature(crypt_str("client.dll"), csgo.signatures.at(10).c_str()));  QUQ;
	hooks::original_setupbones = (DWORD)DetourFunction((PBYTE)setupbones, (PBYTE)hooks::hooked_setupbones);  QUQ;

	static auto clmove = (DWORD)(util::FindSignature(crypt_str("engine.dll"), crypt_str("55 8B EC 81 EC ? ? ? ? 53 56 8A")));  QUQ;
	hooks::original_clmove = (DWORD)DetourFunction((PBYTE)clmove, (PBYTE)hooks::hooked_clmove);  QUQ;

	static auto doextrabonesprocessing = (DWORD)(util::FindSignature(crypt_str("client.dll"), csgo.signatures.at(11).c_str()));  QUQ;
	hooks::original_doextrabonesprocessing = (DWORD)DetourFunction((PBYTE)doextrabonesprocessing, (PBYTE)hooks::hooked_doextrabonesprocessing);  QUQ;

	static auto standardblendingrules = (DWORD)(util::FindSignature(crypt_str("client.dll"), csgo.signatures.at(12).c_str()));  QUQ;
	hooks::original_standardblendingrules = (DWORD)DetourFunction((PBYTE)standardblendingrules, (PBYTE)hooks::hooked_standardblendingrules);  QUQ;

	static auto updateclientsideanimation = (DWORD)(util::FindSignature(crypt_str("client.dll"), csgo.signatures.at(13).c_str()));  QUQ;
	hooks::original_updateclientsideanimation = (DWORD)DetourFunction((PBYTE)updateclientsideanimation, (PBYTE)hooks::hooked_updateclientsideanimation);  QUQ;

	static auto physicssimulate = (DWORD)(util::FindSignature(crypt_str("client.dll"), csgo.signatures.at(14).c_str()));  QUQ;
	hooks::original_physicssimulate = (DWORD)DetourFunction((PBYTE)physicssimulate, (PBYTE)hooks::hooked_physicssimulate);  QUQ;

	static auto modifyeyeposition = (DWORD)(util::FindSignature(crypt_str("client.dll"), csgo.signatures.at(15).c_str()));  QUQ;
	hooks::original_modifyeyeposition = (DWORD)DetourFunction((PBYTE)modifyeyeposition, (PBYTE)hooks::hooked_modifyeyeposition);  QUQ;

	static auto calcviewmodelbob = (DWORD)(util::FindSignature(crypt_str("client.dll"), csgo.signatures.at(16).c_str()));  QUQ;
	hooks::original_calcviewmodelbob = (DWORD)DetourFunction((PBYTE)calcviewmodelbob, (PBYTE)hooks::hooked_calcviewmodelbob);  QUQ;

	static auto shouldskipanimframe = (DWORD)(util::FindSignature(crypt_str("client.dll"), csgo.signatures.at(17).c_str()));  QUQ;
	DetourFunction((PBYTE)shouldskipanimframe, (PBYTE)hooks::hooked_shouldskipanimframe);  QUQ;

	static auto checkfilecrcswithserver = (DWORD)(util::FindSignature(crypt_str("engine.dll"), csgo.signatures.at(18).c_str()));  QUQ;
	DetourFunction((PBYTE)checkfilecrcswithserver, (PBYTE)hooks::hooked_checkfilecrcswithserver);  QUQ;

	static auto processinterpolatedlist = (DWORD)(util::FindSignature(crypt_str("client.dll"), csgo.signatures.at(19).c_str()));  QUQ;
	hooks::original_processinterpolatedlist = (DWORD)DetourFunction((byte*)processinterpolatedlist, (byte*)hooks::processinterpolatedlist);  QUQ;

	hooks::client_hook = new vmthook(reinterpret_cast<DWORD**>(m_client()));  QUQ;
	hooks::client_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_createmove_proxy), 22);  QUQ;
	hooks::client_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_writeusercmddeltatobuffer), 24);  QUQ;
	hooks::client_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_fsn), 37);  QUQ;

	hooks::clientstate_hook = new vmthook(reinterpret_cast<DWORD**>((CClientState*)(uint32_t(m_clientstate()) + 0x8)));  QUQ;
	hooks::clientstate_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_packetstart), 5);  QUQ;
	hooks::clientstate_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_packetend), 6);  QUQ;

	hooks::panel_hook = new vmthook(reinterpret_cast<DWORD**>(m_panel()));  QUQ;
	hooks::panel_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_painttraverse), 41);  QUQ;

	hooks::clientmode_hook = new vmthook(reinterpret_cast<DWORD**>(m_clientmode()));  QUQ;
	hooks::clientmode_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_postscreeneffects), 44);  QUQ;
	hooks::clientmode_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_overrideview), 18);  QUQ;
	hooks::clientmode_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_drawfog), 17);  QUQ;

	hooks::inputinternal_hook = new vmthook(reinterpret_cast<DWORD**>(m_inputinternal()));  QUQ;
	hooks::inputinternal_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_setkeycodestate), 91);  QUQ;
	hooks::inputinternal_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_setmousecodestate), 92);  QUQ;

	hooks::engine_hook = new vmthook(reinterpret_cast<DWORD**>(m_engine()));  QUQ;
	hooks::engine_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_isconnected), 27);  QUQ;
	hooks::engine_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_getscreenaspectratio), 101);  QUQ;
	hooks::engine_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_ishltv), 93);  QUQ;

	hooks::renderview_hook = new vmthook(reinterpret_cast<DWORD**>(m_renderview()));  QUQ;
	hooks::renderview_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_sceneend), 9);  QUQ;

	hooks::materialsys_hook = new vmthook(reinterpret_cast<DWORD**>(m_materialsystem()));  QUQ;
	hooks::materialsys_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_beginframe), 42);  QUQ;
	hooks::materialsys_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_getmaterial), 84);  QUQ;

	hooks::modelrender_hook = new vmthook(reinterpret_cast<DWORD**>(m_modelrender()));  QUQ;
	hooks::modelrender_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_dme), 21);  QUQ;

	hooks::surface_hook = new vmthook(reinterpret_cast<DWORD**>(m_surface()));  QUQ;
	hooks::surface_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_lockcursor), 67);  QUQ;

	hooks::bspquery_hook = new vmthook(reinterpret_cast<DWORD**>(m_engine()->GetBSPTreeQuery()));  QUQ;
	hooks::bspquery_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_listleavesinbox), 6);  QUQ;

	hooks::prediction_hook = new vmthook(reinterpret_cast<DWORD**>(m_prediction()));  QUQ;
	hooks::prediction_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_runcommand), 19);  QUQ;

	hooks::trace_hook = new vmthook(reinterpret_cast<DWORD**>(m_trace()));  QUQ;
	hooks::trace_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_clip_ray_collideable), 4);  QUQ;
	hooks::trace_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_trace_ray), 5);  QUQ;

	hooks::filesystem_hook = new vmthook(reinterpret_cast<DWORD**>(util::FindSignature(crypt_str("engine.dll"), csgo.signatures.at(20).c_str()) + 0x2));  QUQ;
	hooks::filesystem_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_loosefileallowed), 128);  QUQ;

	while (!(INIT::Window = IFH(FindWindow)(crypt_str("Valve001"), nullptr)))
		std::this_thread::sleep_for(std::chrono::milliseconds(100));  QUQ;

	INIT::OldWindow = (WNDPROC)IFH(SetWindowLongPtr)(INIT::Window, GWL_WNDPROC, (LONG_PTR)hooks::Hooked_WndProc);  QUQ;

	hooks::directx_hook = new vmthook(reinterpret_cast<DWORD**>(m_device()));  QUQ;
	hooks::directx_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::Hooked_EndScene_Reset), 16);  QUQ;
	hooks::directx_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::hooked_present), 17);  QUQ;
	hooks::directx_hook->hook_function(reinterpret_cast<uintptr_t>(hooks::Hooked_EndScene), 42);  QUQ;

	hooks::hooked_events.RegisterSelf();  QUQ;

}
