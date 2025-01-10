#pragma once

#include "../../pch.h"

#ifndef LOGIN_H
#define LOGIN_H

namespace login 
{
    class AuthenticationManager
    {
    private:

        ImVec2 login_info_window_pos;
        ImVec2 login_info_window_size;

        bool login_tested;

        std::string file_path = xorstr("C:\\mustinkybot\\user\\license.txt");
        const char* dll_name = xorstr("Dispatcher.dll");

        bool show_information_window;

    public:

        bool show_window = true;
        bool authentication_successful = false;

        bool auto_login_attempted = false;
        bool autologin;

        bool show_update_overlay;
        bool show_loading_overlay;

        char user_license_username[64];
        char user_license_password[64];

        char combined_key[512];

        std::string hwid;
    
        ImVec2 window_size{ 520, 330 };

        AuthenticationManager();

        bool handle_authentication(const char* license_key);
        void show_login_dialog();
        void show_authentication_dialog();
    };
}

#endif
