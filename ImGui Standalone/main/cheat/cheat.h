
#pragma once

#include "../../pch.h"

#ifndef CHEAT_H
#define CHEAT_H

namespace cheat
{
    class CheatManager
    {
    private:

        bool show_login_window;

        bool show_window_overlay;
        bool show_script_overlay;

        bool show_loading_state;

        std::string selected_game;
        std::string selected_game_update;
        std::string selected_game_process;
        std::string selected_game_file;

        bool game_strings_retrieved = false;
        bool loading_dll;

        std::string selected_script;
        std::string selected_script_update;
        std::string selected_script_process;
        std::string selected_script_file;

        bool script_strings_retrieved = false;
        bool loading_script;

        std::string steam = xorstr("Steam.exe");
        std::string battle_net = xorstr("Battle.net.exe");

        ImVec2 window_size{ 470, 320 };

        ImVec2 auth_info_window_pos;
        ImVec2 auth_info_window_size;

       
        std::string additionalText;
        DWORD processId = 0; 

        int inject_dll = 0; 
        int inject_dll_timer = 36000;

        int refresh_timer = clock();
        int refresh_time = 5000;

        int module_scan_refresh_timer = clock();
        int module_scan_refresh_time = 1000;

        enum class LoadingState 
        {
            AwaitingLaunch,
            GameFound,
        };

        LoadingState loading_state = LoadingState::AwaitingLaunch;  

        LoadingState update_loading_state(const std::string& selected_game_process);
        std::string get_text(const LoadingState& loading_state, DWORD processId, const std::string& selected_game_process);

        unsigned char* dllBuffer = nullptr;
        size_t bufferSize = 0;

        bool IsDllFile(const std::string& file_path) 
        {
            return file_path.size() >= 4 && file_path.substr(file_path.size() - 4) == ".dll";
        }

        bool IsExeFile(const std::string& file_path) 
        {
            return file_path.size() >= 4 && file_path.substr(file_path.size() - 4) == ".exe";
        }

        void LaunchExecutable(const std::string& exe_path) 
        {
            ShellExecuteA(NULL, "open", exe_path.c_str(), NULL, NULL, SW_SHOWNORMAL);
        }

        int authenticated_game_count = 0;

    public:

        void handle_game_selection(const std::string& game, const std::string& game_update, const std::string& game_process, const std::string& game_file);

        void handle_popup_game(const std::string& popupMenuId, const std::string& game_process);

        void handle_load_utility(const std::string& game_process);

        void handle_dll_table();
       
        void my_window();
        bool load_dll;

        bool auto_inject;
        bool auto_inject_dll;
    };
}

#endif
