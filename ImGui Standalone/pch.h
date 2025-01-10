#ifndef PCH_H
#define PCH_H

// Windows headers
#include <tchar.h>
#include <Windows.h>

// Standard C++ headers
#include "chrono"
#include <array>
#include <codecvt>
#include <cstdarg>
#include <filesystem>
#include <fstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sddl.h>
#include <sstream>
#include <string>
#include <tlhelp32.h>
#include <unordered_map>
#include <utility>
#include <vector>
#include <random>
#include <winhttp.h>
#include <iphlpapi.h>
#include <WinINet.h>
#include <thread>
#include <psapi.h>
#include <future>
#include <comdef.h>
#include <Wbemidl.h>
#include <map>
#include <unordered_set>

// COM and WMI headers
#include <comutil.h> 
#include <wbemidl.h>

// DirectX headers
#include "d3d11.h"
#include "d3d9.h"

// ImGui headers
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_internal.h"

// Linking to libraries
#pragma comment(lib, "libcurl.lib") 
#pragma comment(lib, "comsuppw.lib") 
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Winhttp.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "VMProtectSDK64.lib")

// Obfuscation headers
#include "obfuscation/xorstr.h"
#include "obfuscation/openssl/sha.h"
#include "obfuscation/decryption/decryption.h"
#include "obfuscation/lazy.h"

#include "obfuscation/VMProtect/include/VMProtectSDK.h"

// Project-specific headers

#include "main/login/login.h"
#include "main/cheat/cheat.h"
#include "drawing/Drawing.h"
#include "utility/hwid/hwid.h"
#include "utility/directory/directory.h"
#include "utility/notifications/notification.h"
#include "utility/flux/flux.hpp"
#include "utility/build/build.h"
#include "utility/protection/protection.h"

// MIB IF TYPE TUNNEL

#ifndef MIB_IF_TYPE_TUNNEL
#define MIB_IF_TYPE_TUNNEL 131
#endif

constexpr int MAX_EXECUTION_TIME_THRESHOLD = 100;

#endif
