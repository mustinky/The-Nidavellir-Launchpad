
#pragma once

#include "../../pch.h"

#ifndef BUILD_H
#define BUILD_H

namespace build
{
    class BuildManager {    

    private:

        int handle_expiry = 0;
        int handle_expiry_timer = 5000;

        DWORD last_injected_processId = 0;

        typedef struct _REFLECTIVE_DLL
        {
            DWORD   SizeOfImage;
            LPVOID  ImageBase;
            LPVOID  EntryPoint;
        } REFLECTIVE_DLL, * PREFLECTIVE_DLL;

    public:

        std::string program_name = xorstr("Nidavellir Launchpad");
        std::string program_version = xorstr("2.0.1");

        void set_mbl_color();

        ImVec2 center_pos(float offsetY = 0.0f);

        void centered_input_text(const char* label, const char* text, char* buffer, size_t bufferSize, float widthMultiplier, float heightMultiplier, bool isPassword = false);
        void centered_button(const char* label, float buttonWidth, bool& toggleValue, bool isButtonDisabled);
        void centered_button_update(const char* label, float buttonWidth, bool& toggleValue);

        void update_loading_overlay(const char* user_license_key, bool& show_loading_overlay, bool shouldShowConnectingScreen);

        void render_large_text(const char* text, ImVec2 center);
        void render_large_text_underlined(const char* text, ImVec2 center);
        void render_medium_text_disabled(const char* text, ImVec2 center);

        void render_loading_circles(const ImVec2& center, float radius, float t, float speed, const ImVec4& color1, const ImVec4& color2);   
        void render_loading_circles_with_text(const char* text, ImVec2 center, float radius, float t, float speed, const ImVec4& color1, const ImVec4& color2);
        void render_rotating_dots(const char* text, const ImVec2& center, float radius, float t, float speed, int dots_count, const ImVec4& color);
        void render_rotating_3d_spheres(const char* text, const ImVec2& center, float radius, float t, float speed, float sphere_radius, const ImVec4& color1, const ImVec4& color2);
        void render_circle(const ImVec2& center, float radius);
        void render_popup(const std::string& title, const std::string& message);

        DWORD get_process_id_from_name(const std::string& processName);

        bool inject_dll_if_different(const std::string& dllPath, DWORD processId);
        bool inject_dll(const std::string& dllPath, DWORD processId);
        bool dll_auth_check();
        bool inject_dll_32_bit(const std::string& dllPath, DWORD processId);

        bool download_a_file(LPCSTR url, LPCSTR directory);
        bool download_and_inject(LPCSTR url, LPCSTR directory, DWORD processID);
        std::wstring utf8_to_wstring(const std::string& utf8str);
        bool download_file(const std::string& url, const std::string& filepath);

        std::string formatted_expiry;
        std::string get_ordinal_suffix(int day);
        std::string format_expiry_date(const std::tm& expiration_tm);
        void handle_expiry_time();

        bool kill_process(const std::string& processName);
        void open_downloaded_file(const std::string& downloadedFilePath);
        void launch_executable(const std::string& executablePath);

        bool process_found(const char* process_name);

        void check_window_state(bool& show_window);

        void check_for_new_update();

        void call_hello_world_message_box(const char* dll_name);
        void call_prevent_tampering(const char* dll_name);

        bool download_file_to_memory(const std::string& url, std::string& memoryBuffer);
        bool inject_DLL_from_memory(HANDLE hProcess, const std::string& buffer);

        bool inject_DLL_from_memory_main(HANDLE hProcess, const unsigned char* dllBuffer, size_t bufferSize);
        bool inject_DLL_from_file_main(HANDLE hProcess, LPCSTR filePath);
        bool download_file_into_memory(LPCSTR url, unsigned char*& buffer, size_t& bufferSize);

        bool inject_dll_from_memory_2(const char* dll_data, size_t dll_size, DWORD processId);
        std::vector<char> download_a_file_to_memory(const char* url);

        BOOL reflective_injection(HANDLE hProcess, LPVOID pReflectiveDllBuffer);
        bool reflective_inject(const std::string& dllLocation, DWORD processId);

        bool is_process_32_bit(DWORD processId);

        const wchar_t* DRIVER_NAME = L"\\Device\\Nidavellir";

        bool load_driver(HANDLE hDevice);
        bool start_driver(HANDLE hDevice);

        void debugt(const std::string& message);

        void draw_pc_name_in_bottom_right(const std::string& pc_name);

        std::string getCurrentDateTime();

        std::string webhook_url = xorstr("not a webhook");
        std::string protection_url = xorstr("nothing to see here");

        bool send_discord_message(const std::string& webhook_url, const std::string& message);
    };
}

#endif