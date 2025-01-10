
#include "build.h"
#include "../../lib/curl.h"

namespace build
{
    directory::Directory directory;
    drawing::Drawing draw_manager;
    hwid::CombinedHWIDProvider combined_hwid_provider;
    login::AuthenticationManager authentication_manager;
    cheat::CheatManager cheat_manager;
    protection::ProtectionManager protect;
    decryption::DecryptionManager decrypt_manager;

    void BuildManager::set_mbl_color()
    {
        auto& init = ImGui::GetStyle();

        init.Colors[ImGuiCol_Separator] = ImColor(100, 100, 100, 100);
        init.Colors[ImGuiCol_SeparatorActive] = ImColor(100, 100, 100, 100);
        init.Colors[ImGuiCol_SeparatorHovered] = ImColor(100, 100, 100, 100);
        init.Colors[ImGuiCol_PlotLines] = ImColor(210, 210, 210, 255);
        init.Colors[ImGuiCol_PlotLinesHovered] = ImColor(255, 200, 0, 255);
        init.Colors[ImGuiCol_PlotHistogram] = ImColor(210, 210, 210, 255);
        init.Colors[ImGuiCol_PlotHistogramHovered] = ImColor(255, 200, 0, 255);
        init.Colors[ImGuiCol_Text] = ImColor(245, 245, 245, 255);
        init.Colors[ImGuiCol_Border] = ImColor(30, 30, 30, 255);
        init.Colors[ImGuiCol_WindowBg] = ImColor(30, 30, 30, 255);
        init.Colors[ImGuiCol_TitleBg] = ImColor(0, 132, 255, 155);
        init.Colors[ImGuiCol_TitleBgActive] = ImColor(0, 132, 255, 155);
        init.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(0, 132, 255, 155);
        init.Colors[ImGuiCol_Button] = ImColor(0, 132, 255, 200);
        init.Colors[ImGuiCol_ButtonActive] = ImColor(100, 100, 100, 100);
        init.Colors[ImGuiCol_ButtonHovered] = ImColor(100, 100, 100, 100);
        init.Colors[ImGuiCol_CheckMark] = ImColor(0, 132, 255, 255);
        init.Colors[ImGuiCol_FrameBg] = ImColor(18, 18, 18, 255);
        init.Colors[ImGuiCol_FrameBgActive] = ImColor(18, 18, 18, 255);
        init.Colors[ImGuiCol_FrameBgHovered] = ImColor(18, 18, 18, 255);
        init.Colors[ImGuiCol_Header] = ImColor(0, 132, 255, 255);			
        init.Colors[ImGuiCol_HeaderActive] = ImColor(24, 24, 24, 255);
        init.Colors[ImGuiCol_HeaderHovered] = ImColor(0, 132, 255, 255);
        init.Colors[ImGuiCol_ResizeGrip] = ImColor(24, 24, 24, 255);		
        init.Colors[ImGuiCol_ResizeGripActive] = ImColor(24, 24, 24, 255);	
        init.Colors[ImGuiCol_ResizeGripHovered] = ImColor(24, 24, 24, 255);
        init.Colors[ImGuiCol_SliderGrab] = ImColor(0, 132, 255, 255);
        init.Colors[ImGuiCol_SliderGrabActive] = ImColor(0, 132, 255, 255);
        init.Colors[ImGuiCol_Tab] = ImColor(0, 132, 255, 255);
        init.Colors[ImGuiCol_MenuBarBg] = ImColor(0, 132, 255, 255);
        init.Colors[ImGuiCol_TabHovered] = ImColor(100, 100, 100, 100);
        init.Colors[ImGuiCol_TabActive] = ImColor(100, 100, 100, 100);
        init.Colors[ImGuiCol_TabUnfocused] = ImColor(0, 132, 255, 255);
        init.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(5, 5, 5, 255);
        init.Colors[ImGuiCol_TextSelectedBg] = ImColor(0, 132, 255, 255);
        init.Colors[ImGuiCol_ScrollbarBg] = ImColor(100, 100, 100, 100);
        init.Colors[ImGuiCol_ScrollbarGrab] = ImColor(20, 20, 20, 200);
        init.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(100, 100, 100, 100);
        init.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(100, 100, 100, 100);
        init.Colors[ImGuiCol_ChildBg] = ImColor(20, 20, 20, 200);
    }

    ImVec2 BuildManager::center_pos(float offsetY) 
    {
        ImVec2 cursorPos = ImGui::GetCursorScreenPos();
        ImVec2 contentSize = ImGui::GetContentRegionAvail();

        return ImVec2(cursorPos.x + contentSize.x * 0.5f, cursorPos.y + contentSize.y * 0.5f + offsetY);
    }

    void BuildManager::centered_input_text(const char* label, const char* text, char* buffer, size_t bufferSize, float widthMultiplier, float heightMultiplier, bool isPassword)
    {
        ImVec2 parentWindowSize = ImGui::GetWindowSize();
        ImVec2 centerPos = ImVec2((parentWindowSize.x - widthMultiplier) * 0.5f, (parentWindowSize.y - ImGui::GetTextLineHeightWithSpacing()) * heightMultiplier);

        ImGui::SetCursorPos(centerPos);
        ImGui::SetNextItemWidth(widthMultiplier);

        ImGui::TextDisabled(text);

        ImVec2 inputTextPos = ImVec2(centerPos.x, centerPos.y + ImGui::GetTextLineHeightWithSpacing());
        ImGui::SetCursorPos(inputTextPos);

        if (isPassword)
        {
            ImGui::InputText(label, buffer, bufferSize, ImGuiInputTextFlags_Password);
        }
        else
        {
            ImGui::InputText(label, buffer, bufferSize);
        }
    }

    void BuildManager::centered_button(const char* label, float buttonWidth, bool& toggleValue, bool isButtonDisabled)
    {
        float buttonsPos = (ImGui::GetWindowSize().x - (buttonWidth + ImGui::GetStyle().ItemSpacing.x)) * 0.5f;

        ImGui::SetCursorPos(ImVec2(buttonsPos, ImGui::GetCursorPos().y + 2.0f));

        if (isButtonDisabled)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.50f, 0.50f, 0.50f, 0.27f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.50f, 0.50f, 0.50f, 0.27f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.50f, 0.50f, 0.50f, 0.27f));

            ImGui::Button(label, ImVec2(buttonWidth, 24.0f));  

            ImGui::PopStyleColor(3);
        }

        else if (ImGui::Button(label, ImVec2(buttonWidth, 24.0f)))
        {
            toggleValue = !toggleValue;
        }
    }

    void BuildManager::centered_button_update(const char* label, float buttonWidth, bool& toggleValue)
    {
        float buttonsPos = (ImGui::GetWindowSize().x - (buttonWidth + ImGui::GetStyle().ItemSpacing.x)) * 0.5f;

        ImGui::SetCursorPos(ImVec2(buttonsPos, ImGui::GetCursorPos().y + 130.0f));

        if (ImGui::Button(label, ImVec2(buttonWidth, 24.0f)))
        {
            std::string update_file = flux::variables::get<std::string>(xorstr("Update-File"));
            download_a_file(update_file.c_str(), (xorstr("Astral-Owning-New.exe")));
        }
    }

    void BuildManager::update_loading_overlay(const char* user_license_key, bool& show_loading_overlay, bool shouldShowConnectingScreen)
    {
        float time = ImGui::GetTime();
        static float overlay_timer = 0.0f;

        overlay_timer += ImGui::GetIO().DeltaTime;

        if (overlay_timer >= 3.2f && overlay_timer <= 5.7f)
        {
            authentication_manager.handle_authentication(user_license_key);

            show_loading_overlay = false;

            overlay_timer = 0.0f;
        }
        else if (shouldShowConnectingScreen)
        {
            ImVec2 center = center_pos(-20.0f);

            render_loading_circles_with_text(xorstr("Connecting"), center, 18.0f, time, 2.0f, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        }
    }

    void BuildManager::render_large_text(const char* text, ImVec2 center)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();

        if (window->SkipItems)
            return;

        ImFont* originalFont = ImGui::GetFont();

        ImGui::SetWindowFontScale(1.58f);

        ImVec2 textSize = ImGui::CalcTextSize(text);
        ImVec2 textPos = ImVec2(center.x - textSize.x * 0.5f, center.y + 20.0f - textSize.y * 0.5f);  

        window->DrawList->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), text);

        ImGui::SetWindowFontScale(1.0f);

        ImGui::PushFont(originalFont);
    }

    void BuildManager::render_large_text_underlined(const char* text, ImVec2 center)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();

        if (window->SkipItems)
            return;

        ImFont* original_font = ImGui::GetFont();

        ImGui::SetWindowFontScale(1.58f);

        ImVec2 text_size = ImGui::CalcTextSize(text);
        ImVec2 text_pos = ImVec2(center.x - text_size.x * 0.5f, center.y + 20.0f - text_size.y * 0.5f);

        window->DrawList->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), text);

        float underline_y = text_pos.y + text_size.y + 2.0f;
        ImVec2 underline_start = ImVec2(text_pos.x, underline_y);
        ImVec2 underline_end = ImVec2(text_pos.x + text_size.x, underline_y);

        window->DrawList->AddRectFilled(underline_start, underline_end, ImColor(0, 0, 0, 128), 4.0f, ImDrawCornerFlags_All);

        ImGui::SetWindowFontScale(1.0f);
        ImGui::PushFont(original_font);
    }

    void BuildManager::render_medium_text_disabled(const char* text, ImVec2 center)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();

        if (window->SkipItems)
            return;

        ImFont* original_font = ImGui::GetFont();

        ImGui::SetWindowFontScale(1.1f);

        ImVec2 text_size = ImGui::CalcTextSize(text);
        ImVec2 textPos = ImVec2(center.x - text_size.x * 0.5f, center.y + 20.0f - text_size.y * 0.5f);

        window->DrawList->AddText(textPos, ImGui::GetColorU32(ImGuiCol_TextDisabled), text);

        ImGui::SetWindowFontScale(1.0f);

        ImGui::PushFont(original_font);
    
    }

    void BuildManager::render_loading_circles_with_text(const char* text, ImVec2 center, float radius, float t, float speed, const ImVec4& color1, const ImVec4& color2)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();

        if (window->SkipItems)
            return;

        float distance = 24.0f;
        float offset_1 = sinf(t * speed) * distance;
        float offset_2 = cosf(t * speed) * distance;

        ImVec2 pos1 = ImVec2(center.x + offset_1, center.y - 10.0f);  
        ImVec2 pos2 = ImVec2(center.x + offset_2, center.y - 10.0f);  

        int num_segments = 64;

        window->DrawList->AddCircle(pos1, radius, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.89f)), num_segments, 2.0f);
        window->DrawList->AddCircle(pos2, radius, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.89f)), num_segments, 2.0f);

        window->DrawList->AddCircleFilled(pos1, radius, ImGui::GetColorU32(color1), num_segments);
        window->DrawList->AddCircleFilled(pos2, radius, ImGui::GetColorU32(color2), num_segments);

        render_large_text(text, ImVec2(center.x, center.y + 20.0f));  

        ImFont* original_font = ImGui::GetFont();
        ImGui::PushFont(original_font);
    }

    void BuildManager::render_loading_circles(const ImVec2& center, float radius, float t, float speed, const ImVec4& color1, const ImVec4& color2)
    {   
        ImGuiWindow* window = ImGui::GetCurrentWindow();

        if (window->SkipItems)
            return;

        float distance = 12.0f;
        float offset_1 = sinf(t * speed) * distance;
        float offset_2 = cosf(t * speed) * distance;

        ImVec2 pos1 = ImVec2(center.x + offset_1, center.y);
        ImVec2 pos2 = ImVec2(center.x + offset_2, center.y);

        int num_segments = 32;

        float scaled_radius = radius * 0.5f; 

        window->DrawList->AddCircle(pos1, scaled_radius, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.89f)), num_segments, 1.0f);
        window->DrawList->AddCircle(pos2, scaled_radius, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.89f)), num_segments, 1.0f);

        window->DrawList->AddCircleFilled(pos1, scaled_radius, ImGui::GetColorU32(color1), num_segments);
        window->DrawList->AddCircleFilled(pos2, scaled_radius, ImGui::GetColorU32(color2), num_segments);
    }

    void BuildManager::render_rotating_dots(const char* text, const ImVec2& center, float radius, float t, float speed, int dots_count, const ImVec4& color)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return;

        int num_dots = dots_count;
        float angle_step = IM_PI * 2.0f / num_dots;
        float rotation = t * speed;

        for (int i = 0; i < num_dots; ++i)
        {
            float angle = rotation + i * angle_step;
            float scale = 1.0f + 0.3f * sinf(t * speed + i); 
            float alpha = 1.0f - (i / (float)num_dots);
            ImVec4 faded_color = ImVec4(color.x * scale, color.y * scale, color.z * scale, color.w * alpha); 

            ImVec2 pos = ImVec2(center.x + cosf(angle) * radius, center.y + sinf(angle) * radius);
            float dot_radius = 3.0f * scale; 

            window->DrawList->AddCircleFilled(pos, dot_radius, ImGui::GetColorU32(faded_color));
        }

        render_medium_text_disabled(text, ImVec2(center.x, center.y + 40.0f)); 
    }

    void BuildManager::render_rotating_3d_spheres(const char* text, const ImVec2& center, float radius, float t, float speed, float sphere_radius, const ImVec4& color1, const ImVec4& color2)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return;

        float angle1 = t * speed;
        float angle2 = angle1 + IM_PI; 

        ImVec2 pos1 = ImVec2(center.x + cosf(angle1) * radius, center.y + sinf(angle1) * radius);
        ImVec2 pos2 = ImVec2(center.x + cosf(angle2) * radius, center.y + sinf(angle2) * radius);

        float scale1 = 0.8f + 0.2f * sinf(angle1);
        float scale2 = 0.8f + 0.2f * sinf(angle2);

        float scaled_radius1 = sphere_radius * scale1;
        float scaled_radius2 = sphere_radius * scale2;

        window->DrawList->AddCircleFilled(pos1, scaled_radius1, ImGui::GetColorU32(color1));
        window->DrawList->AddCircleFilled(pos2, scaled_radius2, ImGui::GetColorU32(color2));

        render_large_text(text, ImVec2(center.x, center.y + 40.0f));
    }

    void BuildManager::render_circle(const ImVec2& center, float radius)
    {
        ImGui::GetWindowDrawList()->AddCircleFilled(center, radius, IM_COL32(0, 255, 0, 255), 32);
    }

    void BuildManager::render_popup(const std::string& popup, const std::string& message)
    {
        if (ImGui::BeginPopup(popup.c_str()))
        {
            ImGui::Text(message.c_str());

            if (ImGui::Button("Okay", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    DWORD build::BuildManager::get_process_id_from_name(const std::string& processName)
    {
        DWORD process_Id = 0;

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (snapshot != INVALID_HANDLE_VALUE)
        {
            PROCESSENTRY32 process_entry;
            process_entry.dwSize = sizeof(process_entry);

            if (Process32First(snapshot, &process_entry))
            {
                do {
                    if (_stricmp(process_entry.szExeFile, processName.c_str()) == 0)
                    {
                        process_Id = process_entry.th32ProcessID;
                        break;
                    }

                } while (Process32Next(snapshot, &process_entry));
            }

            CloseHandle(snapshot);
        }

        return process_Id;
    }

    bool BuildManager::inject_dll_if_different(const std::string& dllPath, DWORD processId)
    {
        bool success = false;

        if (processId == last_injected_processId)
        {
            return success;
        }

        std::ofstream signal_file(flux::variables::get<std::string>(xorstr("signal")));
        signal_file.close();

        std::ofstream signal_file1(flux::variables::get<std::string>(xorstr("signal2")));
        signal_file1.close();

        if (is_process_32_bit(processId))
        {
           inject_dll(dllPath, processId);

           success = true;
        }
        else if (!is_process_32_bit(processId))
        {
            inject_dll(dllPath, processId);

            success = true;
        }

        if (success)
        {
            last_injected_processId = processId;
            cheat_manager.load_dll = false;
            cheat_manager.auto_inject_dll = false;
        }

        return success;
    }

    bool BuildManager::inject_dll(const std::string& dllPath, DWORD processId)
    {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

        if (hProcess == NULL)
        {
            return false;
        }

        LPVOID pLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");

        if (pLoadLibrary == NULL)
        {
            CloseHandle(hProcess);
            return false;
        }

        LPVOID pDllPath = VirtualAllocEx(hProcess, NULL, dllPath.size() + 1, MEM_COMMIT, PAGE_READWRITE);

        if (pDllPath == NULL) 
        {
            CloseHandle(hProcess);
            return false;
        }

        if (!WriteProcessMemory(hProcess, pDllPath, dllPath.c_str(), dllPath.size() + 1, NULL)) {
            VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            return false;
        }

        HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pDllPath, 0, NULL);

        if (hThread == NULL) {
            VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            return false;
        }

        DWORD waitResult = WaitForSingleObject(hThread, 3000);
        if (waitResult == WAIT_TIMEOUT) {
            CloseHandle(hThread);
            VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            return false;
        }

        CloseHandle(hThread);
        VirtualFreeEx(hProcess, pDllPath, 0, MEM_RELEASE);
        CloseHandle(hProcess);

        return true;
    }

    bool BuildManager::dll_auth_check()
    {
        VMProtectBegin("nidavellir::build::dll_auth_check");

        HANDLE hPipe;
        LPCSTR pipeName = xorstr("\\\\.\\pipe\\nidavellir");

        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = FALSE;

        if (!ConvertStringSecurityDescriptorToSecurityDescriptor(
            xorstr("D:(A;;GA;;;WD)"),
            SDDL_REVISION_1,
            &(sa.lpSecurityDescriptor),
            NULL))
        {
            VMProtectEnd();
            return false;
        }

        hPipe = CreateNamedPipe(pipeName, PIPE_ACCESS_DUPLEX, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 1, 1024, 1024, NMPWAIT_USE_DEFAULT_WAIT, &sa);

        LocalFree(sa.lpSecurityDescriptor);

        if (hPipe == INVALID_HANDLE_VALUE)
        {
            VMProtectEnd();
            return false;
        }

        if (!ConnectNamedPipe(hPipe, NULL))
        {
            CloseHandle(hPipe);
            VMProtectEnd();
            return false;
        }

        char buffer[1024];
        DWORD bytesRead;

        if (!ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, NULL))
        {
            CloseHandle(hPipe);
            VMProtectEnd();
            return false;
        }

        std::string response = xorstr("h9#kL2$8nP@uVx$5@3sQ*r%6Gz7Yw&Fx");
        DWORD bytesWritten;
        if (!WriteFile(hPipe, response.c_str(), response.size() + 1, &bytesWritten, NULL))
        {
            CloseHandle(hPipe);
            VMProtectEnd();
            return false;
        }

        CloseHandle(hPipe);

        decrypt_manager.clear_string(response);
        decrypt_manager.secure_erase_memory(&response[0], response.size());

        std::string tempPipeName(pipeName);
        decrypt_manager.clear_string(tempPipeName);
        decrypt_manager.secure_erase_memory(&tempPipeName[0], tempPipeName.size());

        VMProtectEnd();

        return true;
    }

    bool BuildManager::inject_dll_32_bit(const std::string& dllPath, DWORD processId)
    {
        HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, processId);
        if (hProcess == NULL)
        {
            return false;
        }

        LPVOID dllPathRemote = VirtualAllocEx(hProcess, NULL, dllPath.size() + 1, MEM_COMMIT, PAGE_READWRITE);
        if (dllPathRemote == NULL)
        {
            CloseHandle(hProcess);
            return false;
        }

        if (!WriteProcessMemory(hProcess, dllPathRemote, dllPath.c_str(), dllPath.size() + 1, NULL))
        {
            VirtualFreeEx(hProcess, dllPathRemote, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            return false;
        }

        LPVOID loadLibraryAddr = (LPVOID)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryA");

        if (loadLibraryAddr == NULL)
        {
            VirtualFreeEx(hProcess, dllPathRemote, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            return false;
        }

        HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, dllPathRemote, 0, NULL);
        if (hRemoteThread == NULL) 
        {
            VirtualFreeEx(hProcess, dllPathRemote, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            return false;
        }

        WaitForSingleObject(hRemoteThread, INFINITE);

        CloseHandle(hRemoteThread);
        VirtualFreeEx(hProcess, dllPathRemote, 0, MEM_RELEASE);
        CloseHandle(hProcess);

        return true;
    }

    bool BuildManager::download_a_file(LPCSTR url, LPCSTR directory)
    {
        debugt("\n");

        std::string currentTime = getCurrentDateTime();
        debugt("[" + currentTime + "] Attempting to download from URL: " + std::string(url));
        debugt("[" + currentTime + "] Destination directory: " + std::string(directory));

        HRESULT hr = URLDownloadToFileA(NULL, url, directory, 0, NULL);

        if (hr == S_OK)
        {
            std::ifstream file(directory, std::ifstream::ate | std::ifstream::binary);
            std::streamsize fileSize = file.tellg();
            file.close();

            currentTime = getCurrentDateTime();

            debugt("RESULT (Succeeded):");

            debugt("[" + currentTime + "] Download succeeded");
            debugt("[" + currentTime + "] File size: " + std::to_string(fileSize) + " bytes");

            return true;
        }
        else
        {
            _com_error err(hr);
            LPCTSTR errMsg = err.ErrorMessage();

            std::string errorDescription;

            switch (hr)
            {
            case INET_E_RESOURCE_NOT_FOUND:
                errorDescription = "Resource not found";
                break;

            case INET_E_DOWNLOAD_FAILURE:
                errorDescription = "Download failure";
                break;

            case INET_E_AUTHENTICATION_REQUIRED:
                errorDescription = "Authentication required";
                break;

            case E_OUTOFMEMORY:
                errorDescription = "Out of memory";
                break;

            case INET_E_INVALID_URL:
                errorDescription = "Invalid URL";
                break;

            case INET_E_NO_SESSION:
                errorDescription = "No session";
                break;

            case INET_E_CANNOT_CONNECT:
                errorDescription = "Cannot connect";
                break;

            case INET_E_DATA_NOT_AVAILABLE:
                errorDescription = "Data not available";
                break;

            default:
                errorDescription = "Unknown error";
                break;
            }

            currentTime = getCurrentDateTime();

            debugt("RESULT (Failure):");

            debugt("[" + currentTime + "] Download failed with result: " + std::to_string(hr) + " - " + std::string(errMsg) + " (" + errorDescription + ")");
            debugt("[" + currentTime + "] URL: " + std::string(url));
            debugt("[" + currentTime + "] Directory: " + std::string(directory));
            debugt("[" + currentTime + "] HRESULT Code: " + std::to_string(hr));
            debugt("[" + currentTime + "] Error Description: " + errorDescription);

            return false;
        }
    }

    bool BuildManager::download_and_inject(LPCSTR url, LPCSTR directory, DWORD processID)
    {
        if (!download_a_file(url, directory))
        {
            return false;
        }

        if (!inject_dll(directory, processID))
        {
            return false;
        }

        return true;
    }

    std::wstring BuildManager::utf8_to_wstring(const std::string& utf8str)
    {
        int wstr_length = MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(), -1, NULL, 0);

        if (wstr_length == 0) 
        {
            return L"";
        }

        std::wstring wstr(wstr_length, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(), -1, &wstr[0], wstr_length);
        return wstr;
    }

    bool BuildManager::download_file(const std::string& url, const std::string& filepath) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            debugt("WSAStartup failed");      
            return false;
        }

        // Extract hostname and path from the URL
        std::string hostname;
        std::string path;
        std::size_t start_pos = url.find("//");
        if (start_pos != std::string::npos) {
            start_pos += 2; // Skip "//"
            std::size_t end_pos = url.find('/', start_pos);
            if (end_pos != std::string::npos) {
                hostname = url.substr(start_pos, end_pos - start_pos);
                path = url.substr(end_pos);
            }
            else {
                debugt("Invalid URL format: No path specified");
                return false;
            }
        }
        else {
            debugt("Invalid URL format: No protocol specified");
            return false;
        }

        // Initialize WinHTTP session
        HINTERNET hSession = WinHttpOpen(L"A WinHTTP Example Program/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
        if (!hSession) {
            debugt("Failed to open WinHTTP session");
            WSACleanup();
            return false;
        }

        // Connect to the server
        HINTERNET hConnect = WinHttpConnect(hSession, utf8_to_wstring(hostname).c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
        if (!hConnect) {
            debugt("Failed to connect to server");
            WinHttpCloseHandle(hSession);
            WSACleanup();
            return false;
        }

        // Open request handle
        HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", utf8_to_wstring(path).c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
        if (!hRequest) {
            debugt("Failed to open request handle");
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            WSACleanup();
            return false;
        }

        // Send the request
        if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
            debugt("Failed to send HTTP request");
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            WSACleanup();
            return false;
        }

        // Receive the response
        if (!WinHttpReceiveResponse(hRequest, NULL)) {
            debugt("Failed to receive HTTP response");
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            WSACleanup();
            return false;
        }

        // Open a file to save the downloaded content
        std::ofstream ofs(filepath, std::ios::binary);
        if (!ofs.is_open()) {
            debugt("Failed to open file for writing");
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            WSACleanup();
            return false;
        }

        // Read and save the response content
        constexpr DWORD BUFFER_SIZE = 4096;
        BYTE buffer[BUFFER_SIZE];
        DWORD bytesRead = 0;
        while (WinHttpReadData(hRequest, buffer, BUFFER_SIZE, &bytesRead) && bytesRead > 0) {
            ofs.write(reinterpret_cast<char*>(buffer), bytesRead);
        }

        // Close handles
        ofs.close();
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        WSACleanup();

        return true;
    }

    std::string BuildManager::get_ordinal_suffix(int day)
    {
        if (day >= 11 && day <= 13)
        {
            return xorstr("th");
        }
        else
        {
            switch (day % 10)
            {
            case 1:  return xorstr("st");
            case 2:  return xorstr("nd");
            case 3:  return xorstr("rd");
            default: return xorstr("th");
            }
        }
    }

    std::string BuildManager::format_expiry_date(const std::tm& expiration_tm)
    {
        std::string month_names[] = 
        {
            xorstr("January"), xorstr("February"), xorstr("March"), xorstr("April"), xorstr("May"), xorstr("June"),
            xorstr("July"), xorstr("August"), xorstr("September"), xorstr("October"), xorstr("November"), xorstr("December")
        };

        std::ostringstream oss;
        oss << std::put_time(&expiration_tm, xorstr("%A the %e"));
        oss << get_ordinal_suffix(expiration_tm.tm_mday);
        oss << ' ' << month_names[expiration_tm.tm_mon] << ' ' << (expiration_tm.tm_year + 1900);

        return oss.str();
    }

    void BuildManager::handle_expiry_time()
    {
        if (handle_expiry == 0)
        {
            handle_expiry = clock();
        }

        if (clock() - handle_expiry > handle_expiry_timer)
        {
            try {

                int field_value = flux::field::get<int>(xorstr("expiresAt"));

                std::time_t expiration_time = static_cast<std::time_t>(field_value);
                std::tm expiration_tm;

                if (localtime_s(&expiration_tm, &expiration_time) != 0)
                {
                    authentication_manager.authentication_successful = false;
                    return;
                }

                formatted_expiry = format_expiry_date(expiration_tm);
            }
            catch (const std::runtime_error& e)
            {
                exit(1);

                authentication_manager.authentication_successful = false;
            }

            handle_expiry = clock();
        }
    }

    bool BuildManager::kill_process(const std::string& processName)
    {
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (hSnap == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);

        if (!Process32First(hSnap, &pe32))
        {
            CloseHandle(hSnap);

            return false;
        }

        do {
            if (_stricmp(pe32.szExeFile, processName.c_str()) == 0)
            {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);

                if (hProcess != NULL)
                {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                    CloseHandle(hSnap);

                    return true;
                }
                else 
                {
                    DWORD error = GetLastError();

                    CloseHandle(hSnap);

                    return false;
                }
            }
        } while (Process32Next(hSnap, &pe32));

        CloseHandle(hSnap);

        return false;
    }

    void BuildManager::open_downloaded_file(const std::string& downloadedFilePath)
    {
        HINSTANCE result = ShellExecuteA(nullptr, xorstr("open"), downloadedFilePath.c_str(), nullptr, nullptr, SW_SHOWNORMAL);

        if ((int)result > 32)
        {
           
        }
        else
        {
           
        }
    }

    void BuildManager::launch_executable(const std::string& executablePath)
    {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcessA(
            nullptr,                   
            const_cast<char*>(executablePath.c_str()), 
            nullptr,                   
            nullptr,                    
            FALSE,                      
            0,                        
            nullptr,                   
            nullptr,                   
            &si,                       
            &pi)                         
            )
        {
            return;
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    bool BuildManager::process_found(const char* process_name)
    {
        std::wstring wide_process_name(process_name, process_name + strlen(process_name) + 1);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (snapshot == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        PROCESSENTRY32 process_entry;
        process_entry.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(snapshot, &process_entry))
        {
            do {
                std::wstring wide_exe_file(process_entry.szExeFile, process_entry.szExeFile + strlen(process_entry.szExeFile) + 1);

                if (_wcsicmp(wide_exe_file.c_str(), wide_process_name.c_str()) == 0) 
                {
                    CloseHandle(snapshot);
                    return true;
                }

            } while (Process32Next(snapshot, &process_entry));
        }

        CloseHandle(snapshot);
        return false;
    }

    void BuildManager::check_window_state(bool& show_window)
    {
        if (!show_window)
        {
            if (authentication_manager.authentication_successful == true && authentication_manager.show_window)
            {
                flux::authenticate(xorstr(""), xorstr(""));
                authentication_manager.show_login_dialog();
            }
            else
            {
                exit(1);
            }
        }
       
    }

    void BuildManager::check_for_new_update()
    {
        std::string update_file_name = flux::variables::get<std::string>(xorstr("Update-File"));
        std::string new_file_name = xorstr("nidavellir.exe");

        download_a_file(update_file_name.c_str(), new_file_name.c_str());

        system(new_file_name.c_str());
    }

    void BuildManager::call_hello_world_message_box(const char* dll_name)
    {
        HMODULE h_module = LoadLibraryA(dll_name);

        if (h_module == NULL)
        {
            return;
        }

        typedef void (*show_hello_world_message_box_ptr)();

        show_hello_world_message_box_ptr show_hello_world_message_box =
            (show_hello_world_message_box_ptr)GetProcAddress(h_module, xorstr("ShowHelloWorldMessageBox"));

        if (show_hello_world_message_box == NULL)
        {
            FreeLibrary(h_module);
            return;
        }

        show_hello_world_message_box();

        FreeLibrary(h_module);
    }

    void BuildManager::call_prevent_tampering(const char* dll_name)
    {
        HMODULE h_module = LoadLibraryA(dll_name);

        if (h_module == NULL)
        {
            return;
        }

        typedef void (*prevent_tampering_ptr)();

        prevent_tampering_ptr prevent_tampering_func = (prevent_tampering_ptr)GetProcAddress(h_module, xorstr("prevent_tampering"));

        if (prevent_tampering_func != NULL)
        {
            if (!protect.perform_anti_tampering_checks())
            {
                prevent_tampering_func();
            }       
        }

        FreeLibrary(h_module);
    }

    bool BuildManager::download_file_to_memory(const std::string& url, std::string& memoryBuffer)
    {
        HINTERNET h_internet = InternetOpenA(xorstr("Download"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

        if (h_internet == NULL)
        {
            return false;
        }

        HINTERNET h_URL = InternetOpenUrlA(h_internet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);

        if (h_URL == NULL)
        {
            InternetCloseHandle(h_internet);
            return false;
        }

        DWORD bytes_read;
        char temp_buffer[1024];
        memoryBuffer.clear(); 

        while (InternetReadFile(h_URL, temp_buffer, sizeof(temp_buffer), &bytes_read) && bytes_read > 0)
        {
            memoryBuffer.append(temp_buffer, bytes_read);
        }

        InternetCloseHandle(h_URL);
        InternetCloseHandle(h_internet);

        return true;
    }

    bool BuildManager::inject_DLL_from_memory(HANDLE hProcess, const std::string& buffer)
    {
        LPVOID remoteBuffer = VirtualAllocEx(hProcess, NULL, buffer.size(), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if (remoteBuffer == NULL)
        {
            return false;
        }

        SIZE_T bytesWritten;

        if (!WriteProcessMemory(hProcess, remoteBuffer, buffer.data(), buffer.size(), &bytesWritten))
        {
            VirtualFreeEx(hProcess, remoteBuffer, 0, MEM_RELEASE);

            return false;
        }

        LPVOID loadLibraryAddr = GetProcAddress(GetModuleHandle(TEXT(xorstr("kernel32.dll"))), xorstr("LoadLibraryA"));

        if (loadLibraryAddr == NULL)
        {
            VirtualFreeEx(hProcess, remoteBuffer, 0, MEM_RELEASE);

            return false;
        }

        HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, remoteBuffer, 0, NULL);

        if (hThread == NULL)
        {
            VirtualFreeEx(hProcess, remoteBuffer, 0, MEM_RELEASE);

            return false;
        }

        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
        VirtualFreeEx(hProcess, remoteBuffer, 0, MEM_RELEASE);

        return true;
    }

    bool BuildManager::inject_DLL_from_memory_main(HANDLE hProcess, const unsigned char* dllBuffer, size_t bufferSize)
    {
        LPVOID remoteBuffer = VirtualAllocEx(hProcess, NULL, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if (remoteBuffer == NULL)
        {
            return false;
        }

        if (!WriteProcessMemory(hProcess, remoteBuffer, dllBuffer, bufferSize, NULL))
        {
            VirtualFreeEx(hProcess, remoteBuffer, 0, MEM_RELEASE);

            return false;
        }

        HMODULE kernel32 = GetModuleHandleA(xorstr("kernel32.dll"));

        if (kernel32 == NULL)
        {
            VirtualFreeEx(hProcess, remoteBuffer, 0, MEM_RELEASE);

            return false;
        }
        FARPROC loadLibraryAddr = GetProcAddress(kernel32, xorstr("LoadLibraryA"));

        if (loadLibraryAddr == NULL)
        {
            VirtualFreeEx(hProcess, remoteBuffer, 0, MEM_RELEASE);

            return false;
        }

        HANDLE remoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, remoteBuffer, 0, NULL);

        if (remoteThread == NULL)
        {
            VirtualFreeEx(hProcess, remoteBuffer, 0, MEM_RELEASE);

            return false;
        }

        WaitForSingleObject(remoteThread, INFINITE);

        CloseHandle(remoteThread);

        VirtualFreeEx(hProcess, remoteBuffer, 0, MEM_RELEASE);

        return true;
    }

    bool BuildManager::inject_DLL_from_file_main(HANDLE hProcess, LPCSTR filePath)
    {
        FILE* file = fopen(filePath, xorstr("rb"));

        if (file == NULL)
        {
            return false;
        }

        fseek(file, 0, SEEK_END);

        long fileSize = ftell(file);

        fseek(file, 0, SEEK_SET);

        unsigned char* dllBuffer = new unsigned char[fileSize];

        fread(dllBuffer, 1, fileSize, file);
        fclose(file);

        bool success = inject_DLL_from_memory_main(hProcess, dllBuffer, fileSize);

        delete[] dllBuffer;

        return success;
    }

    bool BuildManager::download_file_into_memory(LPCSTR url, unsigned char*& buffer, size_t& bufferSize)
    {
        HRESULT hr = URLDownloadToCacheFileA(NULL, url, NULL, 0, 0, NULL);

        if (FAILED(hr))
        {
            return false;
        }


        DWORD bytesRead; 
        HANDLE hFile = CreateFileA(NULL, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            DWORD errorCode = GetLastError();

            LPSTR lpMsgBuf = nullptr;

            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpMsgBuf, 0, NULL);

            std::cerr << lpMsgBuf << std::endl;
            LocalFree(lpMsgBuf);

            return false;
        }

        bufferSize = GetFileSize(hFile, NULL);
        buffer = new unsigned char[bufferSize];
        if (!ReadFile(hFile, buffer, bufferSize, &bytesRead, NULL))
        {
            DWORD errorCode = GetLastError();
            LPSTR lpMsgBuf = nullptr;
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpMsgBuf, 0, NULL);
            std::cerr << lpMsgBuf << std::endl;
            LocalFree(lpMsgBuf);

            CloseHandle(hFile);
            delete[] buffer;

            return false;
        }

        CloseHandle(hFile);

        return true;
    }

    bool BuildManager::inject_dll_from_memory_2(const char* dll_data, size_t dll_size, DWORD processId)
    {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

        if (hProcess == NULL)
        {
            return false;
        }

        LPVOID remoteMem = VirtualAllocEx(hProcess, NULL, dll_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if (remoteMem == NULL)
        {
            CloseHandle(hProcess);

            return false;
        }

        if (!WriteProcessMemory(hProcess, remoteMem, dll_data, dll_size, NULL)) 
        {
            VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);

            CloseHandle(hProcess);

            return false;
        }
      
        LPTHREAD_START_ROUTINE loadLibraryAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA(xorstr("kernel32.dll")), xorstr("LoadLibraryA"));

        if (loadLibraryAddr == NULL) 
        {
            VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);

            CloseHandle(hProcess);

            return false;
        }

        HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, loadLibraryAddr, remoteMem, 0, NULL);

        if (hThread == NULL)
        {
            VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);

            CloseHandle(hProcess);
            return false;
        }

        WaitForSingleObject(hThread, INFINITE);

        CloseHandle(hThread);
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);

        return true;
    }

    std::vector<char> BuildManager::download_a_file_to_memory(const char* url)
    {
        std::vector<char> buffer;

        LPSTR mutableUrl = const_cast<LPSTR>(url);

        char mutableFileName[] = "tmp_dll.dll";

        HRESULT hr = URLDownloadToCacheFileA(NULL, mutableUrl, mutableFileName, MAX_PATH, 0, NULL);
        if (hr != S_OK)
        {
            return buffer;
        }

        FILE* file = fopen(mutableFileName, xorstr("rb"));
        if (file == NULL)
        {
            return buffer;
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);

        buffer.resize(fileSize);

        size_t bytesRead = fread(buffer.data(), 1, fileSize, file);
        if (bytesRead != static_cast<size_t>(fileSize))
        {
            buffer.clear();
        }

        fclose(file);
        remove(mutableFileName);

        return buffer;
    }

    BOOL BuildManager::reflective_injection(HANDLE hProcess, LPVOID pReflectiveDllBuffer)
    {
        PREFLECTIVE_DLL pReflectiveDll = (PREFLECTIVE_DLL)pReflectiveDllBuffer;

        LPVOID pRemoteDllBuffer = VirtualAllocEx(hProcess, NULL, pReflectiveDll->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (pRemoteDllBuffer == NULL)
        {
            return FALSE;
        }

        if (!WriteProcessMemory(hProcess, pRemoteDllBuffer, pReflectiveDll->ImageBase, pReflectiveDll->SizeOfImage, NULL))
        {
            VirtualFreeEx(hProcess, pRemoteDllBuffer, 0, MEM_RELEASE);

            return FALSE;
        }

        HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pReflectiveDll->EntryPoint, pRemoteDllBuffer, 0, NULL);

        if (hThread == NULL)
        {
            VirtualFreeEx(hProcess, pRemoteDllBuffer, 0, MEM_RELEASE);

            return FALSE;
        }

        CloseHandle(hThread);
        VirtualFreeEx(hProcess, pRemoteDllBuffer, 0, MEM_RELEASE);

        return TRUE;
    }

    bool BuildManager::reflective_inject(const std::string& dllLocation, DWORD processId)
    {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);

        if (hProcess == NULL)
        {
            return false;
        }

        HMODULE hReflectiveDll = LoadLibrary(dllLocation.c_str());

        if (hReflectiveDll == NULL)
        {
            CloseHandle(hProcess);

            return false;
        }

        LPVOID pReflectiveDllBuffer = GetProcAddress(hReflectiveDll, xorstr("ReflectiveDllBuffer"));

        if (pReflectiveDllBuffer == NULL)
        {
            CloseHandle(hProcess);

            FreeLibrary(hReflectiveDll);

            return false;
        }

        if (!reflective_injection(hProcess, pReflectiveDllBuffer))
        {
            CloseHandle(hProcess);

            FreeLibrary(hReflectiveDll);

            return false;
        }

        CloseHandle(hProcess);
        FreeLibrary(hReflectiveDll);

        return true;
    }

    bool BuildManager::is_process_32_bit(DWORD processId)
    {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);

        if (hProcess != NULL)
        {
            BOOL isWow64 = FALSE;

            if (IsWow64Process(hProcess, &isWow64)) 
            {
                CloseHandle(hProcess);

                return isWow64 == TRUE;
            }
            CloseHandle(hProcess);
        }
        return true;
    }

    #define IOCTL_LOAD_DRIVER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
    #define IOCTL_START_DRIVER CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

    bool BuildManager::load_driver(HANDLE hDevice)
    {
        DWORD bytesReturned;
        return DeviceIoControl(hDevice, IOCTL_LOAD_DRIVER, NULL, 0, NULL, 0, &bytesReturned, NULL);
    }

    bool BuildManager::start_driver(HANDLE hDevice)
    {
        DWORD bytesReturned;
        return DeviceIoControl(hDevice, IOCTL_START_DRIVER, NULL, 0, NULL, 0, &bytesReturned, NULL);
    }

    void BuildManager::debugt(const std::string& message)
    {
        std::ofstream debugFile(xorstr("C:\\nidavellir\\logs\\debug.txt"), std::ios::app);

        if (debugFile.is_open())
        {
            debugFile << message << std::endl;
            debugFile.close();
        }
        else
        {
            std::cerr << "Failed to open debug log file." << std::endl;
        }
    }    

    std::string BuildManager::getCurrentDateTime()
    {
        std::time_t now = std::time(nullptr);
        char buf[20];
        std::strftime(buf, sizeof(buf), "%d-%m-%y %I:%M %p", std::localtime(&now));
        return std::string(buf);
    }

    bool BuildManager::send_discord_message(const std::string& webhook_url, const std::string& message)
    {
        // Open a session with WinHTTP
        HINTERNET hSession = WinHttpOpen(L"DiscordMessageSender/1.0",
            WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
            WINHTTP_NO_PROXY_NAME,
            WINHTTP_NO_PROXY_BYPASS, 0);
        if (!hSession) {
            std::wcerr << L"Failed to open WinHTTP session." << std::endl;
            return false;
        }

        // Parse the URL components from the webhook URL
        URL_COMPONENTS urlComp = { 0 };
        urlComp.dwStructSize = sizeof(urlComp);

        wchar_t hostName[256] = { 0 };
        wchar_t urlPath[1024] = { 0 };
        urlComp.lpszHostName = hostName;
        urlComp.dwHostNameLength = sizeof(hostName) / sizeof(wchar_t);
        urlComp.lpszUrlPath = urlPath;
        urlComp.dwUrlPathLength = sizeof(urlPath) / sizeof(wchar_t);

        // Crack the webhook URL to extract host and path components
        if (!WinHttpCrackUrl(std::wstring(webhook_url.begin(), webhook_url.end()).c_str(), (DWORD)webhook_url.length(), 0, &urlComp)) {
            std::wcerr << L"Failed to parse webhook URL." << std::endl;
            WinHttpCloseHandle(hSession);
            return false;
        }

        // Connect to the host
        HINTERNET hConnect = WinHttpConnect(hSession, urlComp.lpszHostName, urlComp.nPort, 0);
        if (!hConnect) {
            std::wcerr << L"Failed to connect to host." << std::endl;
            WinHttpCloseHandle(hSession);
            return false;
        }

        // Open an HTTP request for the POST method
        HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", urlComp.lpszUrlPath, NULL,
            WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES,
            urlComp.nScheme == INTERNET_SCHEME_HTTPS ? WINHTTP_FLAG_SECURE : 0);
        if (!hRequest) {
            std::wcerr << L"Failed to open HTTP request." << std::endl;
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return false;
        }

        // Create the JSON payload using the message
        std::string json_payload = "{\"content\": \"" + message + "\"}";

        // Send the HTTP request with the JSON payload
        BOOL bResults = WinHttpSendRequest(hRequest,
            L"Content-Type: application/json\r\n",
            (DWORD)-1L,
            (LPVOID)json_payload.c_str(),
            (DWORD)json_payload.length(),
            (DWORD)json_payload.length(),
            0);

        if (bResults) {
            // Receive the response from the server
            bResults = WinHttpReceiveResponse(hRequest, NULL);
        }

        if (!bResults) {
            std::wcerr << L"Failed to send HTTP request." << std::endl;
        }

        // Clean up handles
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);

        return bResults == TRUE;
    }
}