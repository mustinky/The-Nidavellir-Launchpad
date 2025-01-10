
#include "login.h"

namespace login
{
    directory::Directory directory;
    drawing::Drawing draw_manager;
    hwid::CombinedHWIDProvider combined_hwid_provider;
    login::AuthenticationManager authentication_manager;
    cheat::CheatManager cheat_manager;
    build::BuildManager build_manager;
    notification::NotificationManager noti_manager;
    decryption::DecryptionManager decrypt_manager;
    protection::ProtectionManager protect;

    namespace fs = std::filesystem;

    static bool show_crosshair = false;

    AuthenticationManager::AuthenticationManager()
    {
       VMProtectBeginUltra("nidavellir::auth::handle::init");

        bool success = flux::set_application(xorstr("clzd6gis80025ns01hd1zss3w"));
        hwid = combined_hwid_provider.get_combined_hwid();

       VMProtectEnd();
    }

    bool AuthenticationManager::handle_authentication(const char* license_key)
    {      
        try
        {
            hwid = hwid::CombinedHWIDProvider().get_combined_hwid();

            VMProtectBeginUltra("nidavellir::auth::handle_authentication");

            flux::authenticate(license_key, hwid.c_str());
      
            char message[512];
            std::snprintf(message, sizeof(message), xorstr("User '%s' (%s) has successfully authenticated."), license_key, hwid.c_str());

            build_manager.send_discord_message(build_manager.webhook_url, message);

           VMProtectEnd();
        }
        catch (const std::runtime_error& e)
        {
            char message[512];
            std::snprintf(message, sizeof(message), xorstr("User '%s' (%s) has encountered an unsuccessful login attempt."), license_key, hwid.c_str());

            build_manager.send_discord_message(build_manager.webhook_url, message);

            return 1;
        }

        try
        {
            const std::string success_msg = flux::variables::get<std::string>("Success");

            if (!success_msg.empty())
            {
                directory.write_license_key_to_file(license_key);
                directory.cache_last_3_chars(license_key);

                authentication_successful = true;

               VMProtectBeginUltra("nidavellir::auth::authentication_dialog");

                show_authentication_dialog();

               VMProtectEnd();
            }
            else
            {
                return 1;
            }
        }
        catch (const std::runtime_error& e)
        {
            return 1;
        }

        return true;
    }

    void AuthenticationManager::show_login_dialog() 
    {
        ImGui::SetNextWindowSize(window_size, ImGuiCond_Once);

        ImGui::Begin(xorstr(" "), &show_window, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        if (show_loading_overlay)
        {        
            if (ImGui::BeginChild(xorstr("LoadingOverlayTabChild"), ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMouseInputs))
            {
                float time = ImGui::GetTime();
                static float overlay_timer = 0.0f;

                overlay_timer += ImGui::GetIO().DeltaTime;

                if (overlay_timer >= 3.2f && overlay_timer <= 5.7f)
                {
                    handle_authentication(combined_key);

                    show_loading_overlay = false;

                    overlay_timer = 0.0f;
                }
                else
                {
                    build_manager.render_rotating_dots(xorstr("Connecting to nidavellir"), build_manager.center_pos(-20.0f), 26.0f, ImGui::GetTime(), 1.4f, 12, ImVec4(0.50f, 0.50f, 0.50f, 0.76f));
                }

                ImGui::EndChild();
            }     
        }
      
        else
        {
            if (ImGui::BeginChild(xorstr("LoadingTabChild"), ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMouseInputs))
            {
                build_manager.render_large_text(build_manager.program_name.c_str(), build_manager.center_pos(-110));

                ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 36, ImGui::GetWindowSize().y - 22));

                ImGui::Text(build_manager.program_version.c_str());

                ImGui::EndChild();
            }

            build_manager.centered_input_text(xorstr("##Username"), xorstr("Username: "), user_license_username, sizeof(user_license_username), 330, 0.4f);
            build_manager.centered_input_text(xorstr("##Password"), xorstr("Password: "), user_license_password, sizeof(user_license_password), 330, 0.55f, true);

            ImGui::Dummy(ImVec2(0.0f, 2.0f));

            build_manager.centered_button(xorstr("Log In"), 185.0f, show_loading_overlay, strlen(user_license_username) == 0 || strlen(user_license_password) == 0);

            snprintf(combined_key, sizeof(combined_key), xorstr("%s:%s"), user_license_username, user_license_password);
        }

        build_manager.check_window_state(show_window);

        ImGui::End();
    }

    void AuthenticationManager::show_authentication_dialog()
    {
        if (!authentication_successful)
        {
            show_login_dialog();                
        }
        else
        {
            directory.create_directories();
            draw_manager.show_success_message();
        }
    }


}
