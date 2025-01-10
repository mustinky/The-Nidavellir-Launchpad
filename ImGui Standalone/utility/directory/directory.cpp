
#include "directory.h"

namespace directory
{
    directory::Directory directory;
    drawing::Drawing draw_manager;
    hwid::CombinedHWIDProvider combined_hwid_provider;
    login::AuthenticationManager authentication_manager;
    cheat::CheatManager cheat_manager;

    bool Directory::create_directories()
    {
        CreateDirectoryW(mustinkybot_path.c_str(), nullptr);
        CreateDirectoryW(user_path.c_str(), nullptr);
        CreateDirectoryW(logs_path.c_str(), nullptr);
        CreateDirectoryW(settings_path.c_str(), nullptr);
        CreateDirectoryW(dll_path.c_str(), nullptr);

        return true;
    }

    bool Directory::remove_directories()
    {
        bool success = true;

        if (!RemoveDirectoryW(dll_path.c_str()))
            success = false;

        if (!RemoveDirectoryW(settings_path.c_str()))
            success = false;

        if (!RemoveDirectoryW(logs_path.c_str()))
            success = false;

        if (!RemoveDirectoryW(user_path.c_str()))
            success = false;

        if (!RemoveDirectoryW(mustinkybot_path.c_str()))
            success = false;

        return success;
    }

    bool Directory::read_auto_login_setting()
    {
        std::wstring settings_file_path = settings_path;
        std::ifstream settings_file(settings_file_path);

        if (settings_file.is_open())
        {
            std::string line;

            while (std::getline(settings_file, line))
            {
                if (line.find(xorstr("AutoLogin = true")) != std::string::npos)
                {
                    authentication_manager.autologin = true;

                    break;
                }
            }

            settings_file.close();
        }

        return authentication_manager.autologin;
    }

    bool Directory::read_license_key()
    {
       
    }

    bool Directory::create_folder_exclusion()
    {
        const std::wstring folder_path = L"C:\\mustinkybot";

        HKEY hKey;
        LONG result = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows Defender\\Exclusions\\Paths", 0, KEY_SET_VALUE | KEY_WOW64_64KEY, &hKey);

        if (result == ERROR_SUCCESS)
        {

            result = RegSetValueExW(hKey, L"MustinkyBotFolderExclusion", 0, REG_SZ, reinterpret_cast<const BYTE*>(folder_path.c_str()), static_cast<DWORD>((folder_path.size() + 1) * sizeof(wchar_t)));

            if (result == ERROR_SUCCESS)
            {
                RegCloseKey(hKey);
                return true;
            }
            else
            {
            }
        }
        else
        {
        }

        return false;
    }

    typedef LONG NTSTATUS;
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)

    void Directory::exclude_folder_from_defender(const wchar_t* folderPath) {

        HANDLE hMsftMpHandle;
        if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hMsftMpHandle))
        {
            if (GetLastError() == ERROR_NO_TOKEN)

                if (!ImpersonateSelf(SecurityImpersonation))
                {

                    return;
                }
        }
        else
            CloseHandle(hMsftMpHandle);

        HANDLE hMsftMpProcessToken = nullptr;

        if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hMsftMpProcessToken))
        {
            LUID luid;
            if (LookupPrivilegeValue(nullptr, "SeDebugPrivilege", &luid))
            {
                TOKEN_PRIVILEGES tokenPrivileges;

                tokenPrivileges.PrivilegeCount = 1;
                tokenPrivileges.Privileges[0].Luid = luid;
                tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

                AdjustTokenPrivileges(hMsftMpProcessToken, FALSE, &tokenPrivileges, sizeof(TOKEN_PRIVILEGES), nullptr, nullptr);
                DWORD adjustTokenError = GetLastError();

                if (adjustTokenError == ERROR_SUCCESS)
                {
                }
                else
                {
                }
            }

            CloseHandle(hMsftMpProcessToken);
        }

        wchar_t command[MAX_PATH + 100];
        swprintf_s(command, sizeof(command) / sizeof(wchar_t), L"%s\\%s %s", L"C:\\Program Files\\Windows Defender\\mpcmdrun.exe", L"MpCmdRun.exe", L"-AddExclusion");
        wcscat_s(command, MAX_PATH + 100, L" -ExclusionPath ");
        wcscat_s(command, MAX_PATH + 100, folderPath);

        STARTUPINFOW si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(STARTUPINFOW));
        ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
        si.cb = sizeof(STARTUPINFOW);

        if (CreateProcessW(nullptr, command, nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi))
        {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        else
        {
        }
    }

    std::string Directory::get_module_file_location(DWORD process_id, const std::string& module_name)
    {
        HMODULE hMods[1024];
        HANDLE hProcess;
        DWORD cbNeeded;
        wchar_t process_name[MAX_PATH];

        hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_id);

        if (hProcess == NULL)
        {
            return xorstr("");

        }

        if (!GetModuleFileNameExW(hProcess, NULL, process_name, sizeof(process_name) / sizeof(wchar_t)))
        {
            CloseHandle(hProcess);
            return xorstr("");
        }

        if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
            for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
                wchar_t module_name_buffer[MAX_PATH];
                if (GetModuleFileNameExW(hProcess, hMods[i], module_name_buffer, sizeof(module_name_buffer) / sizeof(wchar_t))) {
                    std::wstring module_name_wstr(module_name_buffer);
                    std::string module_name_str(module_name_wstr.begin(), module_name_wstr.end());
                    size_t found = module_name_str.find_last_of(xorstr("\\"));
                    std::string module_path = module_name_str.substr(0, found);
                    std::string module_filename = module_name_str.substr(found + 1);

                    if (module_filename == module_name) {
                        CloseHandle(hProcess);
                        return module_path;
                    }
                }
            }
        }

        CloseHandle(hProcess);

        return xorstr("");
    }

    std::string Directory::check_for_module_file_location(DWORD process_id, const std::string& module_name)
    {
        std::string module_path;

        while (module_path.empty()) 
        {
            module_path = check_for_module_file_location_internal(process_id, module_name);

            if (module_path.empty())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
            }
        }

        return module_path;
    }

    bool Directory::check_for_module_file_location_internal(DWORD process_id, const std::string& module_name)
    {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);

        if (hSnapshot == INVALID_HANDLE_VALUE)
        {
            // Error creating snapshot
            return false;
        }

        MODULEENTRY32 moduleEntry;
        moduleEntry.dwSize = sizeof(MODULEENTRY32);

        bool moduleFound = false;

        if (Module32First(hSnapshot, &moduleEntry))
        {
            do {
                if (_stricmp(moduleEntry.szModule, module_name.c_str()) == 0)
                {
                    moduleFound = true;
                    break;
                }
            } while (Module32Next(hSnapshot, &moduleEntry));
        }

        CloseHandle(hSnapshot);

        return moduleFound;
    }

    void Directory::create_text_file(const std::string& directory, const std::string& file_name, const std::string& content)
    {
        std::ofstream outfile(directory + xorstr("\\") + file_name);

        if (outfile.is_open())
        {
            outfile << content;
            outfile.close();
        }
    }

    void Directory::main_function(DWORD process_id, const std::string& module_name)
    {
        std::string module_directory = get_module_file_location(process_id, module_name);

        if (!module_directory.empty())
        {
            create_text_file(module_directory, xorstr("test.txt"), xorstr("This is a test file."));
        }
    }

    int Directory::get_IO_priority(DWORD processId)
    {
        HRESULT hres;

        hres = CoInitializeEx(0, COINIT_MULTITHREADED);
        if (FAILED(hres)) {
            return -1;
        }

        // Initialize security settings
        hres = CoInitializeSecurity(
            NULL,
            -1,                          // COM authentication
            NULL,                        // Authentication services
            NULL,                        // Reserved
            RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
            RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
            NULL,                        // Authentication info
            EOAC_NONE,                   // Additional capabilities 
            NULL                         // Reserved 
        );

        if (FAILED(hres)) {
            CoUninitialize();
            return -1;
        }

        IWbemLocator* pLoc = NULL;

        hres = CoCreateInstance(
            CLSID_WbemLocator,
            0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator, (LPVOID*)&pLoc);

        if (FAILED(hres)) {
            CoUninitialize();
            return -1;
        }

        IWbemServices* pSvc = NULL;

        hres = pLoc->ConnectServer(
            _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
            NULL,                    // User name
            NULL,                    // User password
            0,                       // Locale
            NULL,                    // Security flags
            0,                       // Authority
            0,                       // Context object
            &pSvc                    // IWbemServices proxy
        );

        if (FAILED(hres)) {
            pLoc->Release();
            CoUninitialize();
            return -1;
        }

        hres = CoSetProxyBlanket(
            pSvc,                         // Indicates the proxy to set
            RPC_C_AUTHN_WINNT,            // RPC_C_AUTHN_xxx
            RPC_C_AUTHZ_NONE,             // RPC_C_AUTHZ_xxx
            NULL,                         // Server principal name 
            RPC_C_AUTHN_LEVEL_CALL,       // RPC_C_AUTHN_LEVEL_xxx 
            RPC_C_IMP_LEVEL_IMPERSONATE,  // RPC_C_IMP_LEVEL_xxx
            NULL,                         // client identity
            EOAC_NONE                     // proxy capabilities 
        );

        if (FAILED(hres)) {
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return -1;
        }

        IEnumWbemClassObject* pEnumerator = NULL;
        hres = pSvc->ExecQuery(
            bstr_t("WQL"),
            bstr_t("SELECT * FROM Win32_Process WHERE ProcessId = <processId>"), 
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator);

        if (FAILED(hres)) {
            pSvc->Release();
            pLoc->Release();
            CoUninitialize();
            return -1;
        }

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;
        while (pEnumerator) {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
            if (0 == uReturn) {
                break;
            }

            VARIANT vtProp;
            hr = pclsObj->Get(L"Priority", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hr)) {
                return vtProp.intVal; 
            }

            VariantClear(&vtProp);
            pclsObj->Release();
        }

        pEnumerator->Release();
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();

        return -1;
    }

    bool Directory::process_memory_usage(DWORD processId)
    {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
        if (hProcess == NULL)
        {
            return false;
        }

        PROCESS_MEMORY_COUNTERS_EX pmc;
        if (!GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)))
        {
            CloseHandle(hProcess);
            return false;
        }

        CloseHandle(hProcess);

        if (pmc.PrivateUsage > 3LL * 1024 * 1024 * 1024)
        {
            return true;
        }

        return false;
    }

    bool Directory::is_process_active(DWORD processId)
    {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processId);
        if (hProcess == NULL) 
        {
            return false;
        }

        DWORD exitCode = 0;

        if (!GetExitCodeProcess(hProcess, &exitCode))
        {
            CloseHandle(hProcess);
            return false;
        }

        CloseHandle(hProcess);

        return exitCode == STILL_ACTIVE;
    }

    bool Directory::write_license_key_to_file(const char* license_key)
    {
        std::ofstream file(file_path);
        if (file.is_open())
        {
            file << license_key;
            file.close();
            return true;
        }
        return false;
    }

    void Directory::auto_login_if_key_exists()
    {
        if (!auto_login_attempted)
        {
            std::ifstream file(file_path);
            if (file.good())
            {
                char key[64];
                file.getline(key, sizeof(key));
                file.close();

                if (authentication_manager.handle_authentication(key))
                {
                    auto_login_attempted = true;
                }
            }
        }
    }

    void Directory::cache_last_3_chars(const char* license_key)
    {
        if (std::strlen(license_key) >= 3)
        {
            cached_last_3_chars.push_back(std::string(license_key + std::strlen(license_key) - 3));
        }
        else
        {
            cached_last_3_chars.push_back("");
        }
    }

    const std::string& Directory::get_cached_last_3_chars(size_t index) const
    {
        if (index < cached_last_3_chars.size())
        {
            return cached_last_3_chars[index];
        }
        else
        {
            static const std::string empty_str = "";
            return empty_str;
        }
    }
}


