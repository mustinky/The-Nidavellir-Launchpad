
#include "hwid.h"

namespace hwid {

    directory::Directory directory;
    drawing::Drawing draw_manager;
    hwid::CombinedHWIDProvider combined_hwid_provider;
    login::AuthenticationManager authentication_manager;
    cheat::CheatManager cheat_manager;

    std::string CPUIDProvider::get_cpu_id()
    {
        std::string cpu_id;

        if (CoInitializeEx(0, COINIT_MULTITHREADED) == S_OK)
        {
            if (CoInitializeSecurity(
                nullptr,
                -1,
                nullptr,
                nullptr,
                RPC_C_AUTHN_LEVEL_DEFAULT,
                RPC_C_IMP_LEVEL_IMPERSONATE,
                nullptr,
                EOAC_NONE,
                nullptr) == S_OK)
            {
                IWbemLocator* p_loc = nullptr;
                IWbemServices* p_svc = nullptr;

                if (CoCreateInstance(
                    CLSID_WbemLocator,
                    0,
                    CLSCTX_INPROC_SERVER,
                    IID_IWbemLocator,
                    reinterpret_cast<LPVOID*>(&p_loc)) == S_OK)
                {
                    if (p_loc->ConnectServer(
                        BSTR(L"ROOT\\CIMV2"),
                        nullptr,
                        nullptr,
                        nullptr,
                        0,
                        nullptr,
                        nullptr,
                        &p_svc) == S_OK) 
                    {
                        if (CoSetProxyBlanket(
                            p_svc,
                            RPC_C_AUTHN_WINNT,
                            RPC_C_AUTHZ_NONE,
                            nullptr,
                            RPC_C_AUTHN_LEVEL_CALL,
                            RPC_C_IMP_LEVEL_IMPERSONATE,
                            nullptr,
                            EOAC_NONE) == S_OK)
                        {
                            IEnumWbemClassObject* p_enumerator = nullptr;

                            if (p_svc->ExecQuery(
                                BSTR(L"WQL"),
                                BSTR(L"SELECT * FROM Win32_Processor"),
                                WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                                nullptr,
                                &p_enumerator) == S_OK)
                            {
                                IWbemClassObject* p_cls_obj = nullptr;
                                ULONG u_return = 0;

                                while (p_enumerator) 
                                {
                                    HRESULT hr = p_enumerator->Next(WBEM_INFINITE, 1, &p_cls_obj, &u_return);

                                    if (u_return == 0) 
                                    {
                                        break;
                                    }

                                    VARIANT vt_prop;

                                    hr = p_cls_obj->Get(L"ProcessorId", 0, &vt_prop, 0, 0);

                                    if (hr == S_OK) 
                                    {
                                        cpu_id = _bstr_t(vt_prop.bstrVal);
                                        VariantClear(&vt_prop);
                                    }

                                    p_cls_obj->Release();
                                }
                                p_enumerator->Release();
                            }
                        }
                    }
                }

                if (p_svc)
                {
                    p_svc->Release();
                }
                if (p_loc)
                {
                    p_loc->Release();
                }

                CoUninitialize();
            }
        }

        return cpu_id;
    }

    std::string MACAddressProvider::get_mac_address()
    {
        std::string mac_address;

        if (CoInitializeEx(0, COINIT_MULTITHREADED) == S_OK)
        {
            if (CoInitializeSecurity(nullptr, -1, nullptr, nullptr, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE, nullptr) == S_OK) {
                IWbemLocator* p_loc = nullptr;
                IWbemServices* p_svc = nullptr;

                if (CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<LPVOID*>(&p_loc)) == S_OK)
                {
                    if (p_loc->ConnectServer(BSTR(L"ROOT\\CIMV2"), nullptr, nullptr, nullptr, 0, nullptr, nullptr, &p_svc) == S_OK)
                    {
                        if (CoSetProxyBlanket(p_svc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE) == S_OK)
                        {
                            IEnumWbemClassObject* p_enumerator = nullptr;

                            if (p_svc->ExecQuery(BSTR(L"WQL"), BSTR(L"SELECT * FROM Win32_NetworkAdapterConfiguration WHERE MACAddress IS NOT NULL AND IPEnabled = true"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, nullptr, &p_enumerator) == S_OK)
                            {
                                IWbemClassObject* p_cls_obj = nullptr;
                                ULONG u_return = 0;

                                while (p_enumerator)
                                {
                                    HRESULT hr = p_enumerator->Next(WBEM_INFINITE, 1, &p_cls_obj, &u_return);

                                    if (u_return == 0) {
                                        break;
                                    }

                                    VARIANT vt_prop;

                                    hr = p_cls_obj->Get(L"MACAddress", 0, &vt_prop, 0, 0);

                                    if (hr == S_OK) {
                                        mac_address = _bstr_t(vt_prop.bstrVal);
                                        VariantClear(&vt_prop);
                                        break;
                                    }

                                    p_cls_obj->Release();
                                }
                                p_enumerator->Release();
                            }
                        }
                    }
                }

                if (p_svc) {
                    p_svc->Release();
                }
                if (p_loc) {
                    p_loc->Release();
                }

                CoUninitialize();
            }
        }

        mac_address.erase(std::remove(mac_address.begin(), mac_address.end(), ':'), mac_address.end());

        return mac_address;
    }

    std::string VSProvider::get_volume_serial()
    {
        DWORD serial_number;
        if (GetVolumeInformationW(L"C:\\", nullptr, 0, &serial_number, nullptr, nullptr, nullptr, 0))
        {
            std::wstringstream wss;
            wss << serial_number;
            std::wstring wstr_serial = wss.str();

            int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr_serial.c_str(), -1, nullptr, 0, nullptr, nullptr);
            std::string result(size_needed, 0);
            WideCharToMultiByte(CP_UTF8, 0, wstr_serial.c_str(), -1, &result[0], size_needed, nullptr, nullptr);

            return result;
        }
        return "";
    }

    std::string DIProvider::get_disk_info()
    {
        DWORD serial_number;
        if (GetVolumeInformationW(L"C:\\", nullptr, 0, &serial_number, nullptr, nullptr, nullptr, 0))
        {
            std::wstringstream wss;
            wss << std::hex << std::setw(8) << std::setfill(L'0') << serial_number;
            std::wstring wstr_serial = wss.str();

            int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr_serial.c_str(), -1, nullptr, 0, nullptr, nullptr);
            std::string result(size_needed, 0);
            WideCharToMultiByte(CP_UTF8, 0, wstr_serial.c_str(), -1, &result[0], size_needed, nullptr, nullptr);

            return result;
        }
        return "";
    }

    std::string BSProvider::get_bios_serial()
    {
        std::string bios_serial;

        if (CoInitializeEx(0, COINIT_MULTITHREADED) == S_OK)
        {
            if (CoInitializeSecurity(
                nullptr,
                -1,
                nullptr,
                nullptr,
                RPC_C_AUTHN_LEVEL_DEFAULT,
                RPC_C_IMP_LEVEL_IMPERSONATE,
                nullptr,
                EOAC_NONE,
                nullptr) == S_OK)
            {
                IWbemLocator* p_loc = nullptr;
                IWbemServices* p_svc = nullptr;

                if (CoCreateInstance(
                    CLSID_WbemLocator,
                    0,
                    CLSCTX_INPROC_SERVER,
                    IID_IWbemLocator,
                    reinterpret_cast<LPVOID*>(&p_loc)) == S_OK)
                {
                    if (p_loc->ConnectServer(
                        BSTR(L"ROOT\\CIMV2"),
                        nullptr,
                        nullptr,
                        nullptr,
                        0,
                        nullptr,
                        nullptr,
                        &p_svc) == S_OK)
                    {
                        if (CoSetProxyBlanket(
                            p_svc,
                            RPC_C_AUTHN_WINNT,
                            RPC_C_AUTHZ_NONE,
                            nullptr,
                            RPC_C_AUTHN_LEVEL_CALL,
                            RPC_C_IMP_LEVEL_IMPERSONATE,
                            nullptr,
                            EOAC_NONE) == S_OK)
                        {
                            IEnumWbemClassObject* p_enumerator = nullptr;

                            if (p_svc->ExecQuery(
                                BSTR(L"WQL"),
                                BSTR(L"SELECT * FROM Win32_BIOS"),
                                WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                                nullptr,
                                &p_enumerator) == S_OK)
                            {
                                IWbemClassObject* p_cls_obj = nullptr;
                                ULONG u_return = 0;

                                if (p_enumerator->Next(WBEM_INFINITE, 1, &p_cls_obj, &u_return) == S_OK)
                                {
                                    VARIANT vt_prop;
                                    HRESULT hr = p_cls_obj->Get(L"SerialNumber", 0, &vt_prop, 0, 0);

                                    if (hr == S_OK)
                                    {
                                        bios_serial = _bstr_t(vt_prop.bstrVal);
                                        VariantClear(&vt_prop);
                                    }

                                    p_cls_obj->Release();
                                }

                                p_enumerator->Release();
                            }
                        }
                    }
                }

                if (p_svc)
                {
                    p_svc->Release();
                }
                if (p_loc)
                {
                    p_loc->Release();
                }

                CoUninitialize();
            }
        }

        return bios_serial;
    }

    std::string IPProvider::get_internal_ip()
    {
        std::string internal_ip;

        ULONG buflen = 0;

        if (GetAdaptersInfo(nullptr, &buflen) == ERROR_BUFFER_OVERFLOW)
        {
            std::unique_ptr<BYTE[]> buffer(new BYTE[buflen]);

            PIP_ADAPTER_INFO pAdapterInfo = reinterpret_cast<PIP_ADAPTER_INFO>(buffer.get());
            if (GetAdaptersInfo(pAdapterInfo, &buflen) == NO_ERROR)
            {
                while (pAdapterInfo)
                {
                    if (pAdapterInfo->Type != MIB_IF_TYPE_LOOPBACK && pAdapterInfo->Type != MIB_IF_TYPE_TUNNEL)
                    {
                        internal_ip = pAdapterInfo->IpAddressList.IpAddress.String;
                        break;
                    }
                    pAdapterInfo = pAdapterInfo->Next;
                }
            }
        }

        internal_ip.erase(std::remove(internal_ip.begin(), internal_ip.end(), '.'), internal_ip.end());

        return internal_ip;
    }

    std::string IPProvider::get_external_ip()
    {
        std::string external_ip;

        HINTERNET hSession = WinHttpOpen(L"Astral-Owning/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

        if (hSession) 
        {
            HINTERNET hConnect = WinHttpConnect(hSession, L"api.ipify.org", INTERNET_DEFAULT_HTTP_PORT, 0);

            if (hConnect)
            {
                HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/", nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

                if (hRequest)
                {
                    if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
                    {
                        if (WinHttpReceiveResponse(hRequest, nullptr))
                        {
                            DWORD dwSize = 0;
                            DWORD dwDownloaded = 0;
                            LPSTR pszOutBuffer;

                            do
                            {
                                dwSize += 1024;

                                pszOutBuffer = new char[dwSize + 1];
                                if (pszOutBuffer)
                                {
                                    if (WinHttpReadData(hRequest, pszOutBuffer + dwDownloaded, 1024, &dwDownloaded))
                                    {
                                        pszOutBuffer[dwDownloaded] = '\0';
                                        external_ip += pszOutBuffer;
                                    }
                                    delete[] pszOutBuffer;
                                }
                            } while (dwDownloaded != 0);
                        }
                    }
                    WinHttpCloseHandle(hRequest);
                }
                WinHttpCloseHandle(hConnect);
            }
            WinHttpCloseHandle(hSession);
        }

        external_ip.erase(std::remove(external_ip.begin(), external_ip.end(), '.'), external_ip.end());

        return external_ip;
    }

    std::string PCNameProvider::get_pc_name()
    {
        char buffer[MAX_COMPUTERNAME_LENGTH + 1];
        DWORD size = sizeof(buffer);

        if (!GetComputerNameA(buffer, &size))
        {
            return xorstr("");
        }

        return std::string(buffer);
    }

    std::string CombinedHWIDProvider::get_combined_hwid()
    {
        VMProtectBeginUltra("nidavellir::auth::hwid::get");

        std::string cpu_id = CPUIDProvider::get_cpu_id();
        std::string mac_address = MACAddressProvider::get_mac_address();
        std::string volume_serial = VSProvider::get_volume_serial();
        std::string disk_info = DIProvider::get_disk_info();
        std::string bios_serial = BSProvider::get_bios_serial();

        std::string combined_hwid = cpu_id + mac_address + volume_serial + disk_info + bios_serial;

        std::seed_seq seed(combined_hwid.begin(), combined_hwid.end());
        std::mt19937 g(seed);

        std::shuffle(combined_hwid.begin(), combined_hwid.end(), g);

        const size_t max_length = 50;
        const int max_hwid_size = 256; 

        char vm_hwid[max_hwid_size];
        VMProtectGetCurrentHWID(vm_hwid, max_hwid_size);

        return combined_hwid;

       VMProtectEnd();
    }
}
