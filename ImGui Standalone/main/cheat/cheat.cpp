
#include "cheat.h"

namespace cheat
{
    directory::Directory directory;
    drawing::Drawing draw_manager;
    hwid::CombinedHWIDProvider combined_hwid_provider;
    login::AuthenticationManager authentication_manager;
    cheat::CheatManager cheat_manager;
    build::BuildManager build_manager;
    notification::NotificationManager noti_manager;
    decryption::DecryptionManager decrypt_manager;
    hwid::PCNameProvider pc_name;

    std::string hwid_ = hwid::CombinedHWIDProvider().get_combined_hwid();

    static std::vector<std::string> all_game_strings;

    void CheatManager::handle_game_selection(const std::string& game, const std::string& game_update, const std::string& game_process, const std::string& game_file)
    {
        std::string popup_id = xorstr("##") + game;

        if (ImGui::Selectable(game.c_str(), false, ImGuiSelectableFlags_SpanAllColumns))
        {
            selected_game = game;
            selected_game_update = game_update;
            selected_game_process = game_process;
            selected_game_file = game_file;

            ImGui::OpenPopup(popup_id.c_str());
        }

        ImGui::NextColumn();

        ImGui::TextDisabled(game_update.c_str());

        ImGui::NextColumn();

        handle_popup_game(popup_id, game_process);
    }

    void CheatManager::handle_popup_game(const std::string& popup, const std::string& game_process)
    {
        if (ImGui::BeginPopup(popup.c_str()))
        {
            ImGui::Text(selected_game.c_str());

            ImGui::Separator();
                   
            if (!build_manager.process_found(game_process.c_str()))
            {
                if (ImGui::MenuItem(xorstr("load utility")))
                    handle_load_utility(game_process);           
            }

            else if (build_manager.process_found(game_process.c_str()))
            {
                if (ImGui::MenuItem(xorstr("close game")))
                    build_manager.kill_process(game_process.c_str());          
            }
         
            ImGui::EndPopup();
        }
    }

    void CheatManager::handle_load_utility(const std::string& game_process)
    {
        if (build_manager.process_found(game_process.c_str()))
        {
            build_manager.kill_process(game_process.c_str());
        }
    
        show_window_overlay = !show_window_overlay;
    }

    void CheatManager::handle_dll_table()
    {
      VMProtectBeginUltra("nidavellir::auth::handle::content");

        if (game_strings_retrieved)
            return;
     
        std::string dll = flux::variables::get<std::string>(xorstr("DLL"));

        std::vector<std::string> games;

        size_t pos = 0;

        while ((pos = dll.find(xorstr(":"))) != std::string::npos) 
        {
            games.push_back(dll.substr(0, pos));

            dll.erase(0, pos + 1);
        }

        games.push_back(dll);

        for (const std::string& game : games)
        {
            std::string game_string = flux::variables::get<std::string>(game);

            std::string game_string_update = flux::variables::get<std::string>(game + xorstr("-Update"));
            std::string game_string_process = flux::variables::get<std::string>(game + xorstr("-Process"));
            std::string game_string_file = flux::variables::get<std::string>(game + xorstr("-File"));

            all_game_strings.push_back(game_string);
            all_game_strings.push_back(game_string_update);
            all_game_strings.push_back(game_string_process);
            all_game_strings.push_back(game_string_file);

            decrypt_manager.clear_string(game_string);
            decrypt_manager.clear_string(game_string_update);
            decrypt_manager.clear_string(game_string_process);
            decrypt_manager.clear_string(game_string_file);

            decrypt_manager.secure_erase_memory(&game_string[0], game_string.size());
            decrypt_manager.secure_erase_memory(&game_string_update[0], game_string_update.size());
            decrypt_manager.secure_erase_memory(&game_string_process[0], game_string_process.size());
            decrypt_manager.secure_erase_memory(&game_string_file[0], game_string_file.size());

            authenticated_game_count++;
        }

        char message[512];
        std::snprintf(message, sizeof(message), xorstr("User '%s' has successfully authenticated on %d game(s)."), hwid_.c_str(), authenticated_game_count);

        build_manager.send_discord_message(build_manager.webhook_url, message);

        game_strings_retrieved = true;

      VMProtectEnd();
    }

    void CheatManager::my_window()
    {
        try {

            build_manager.handle_expiry_time();

            handle_dll_table();

            ImGui::SetNextWindowSize(window_size, ImGuiCond_Once);

            ImGui::Begin(xorstr(" "), &authentication_manager.show_window, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

            if (show_window_overlay)
            {          
                if (clock() - refresh_timer > refresh_time)
                {
                    processId = build_manager.get_process_id_from_name(selected_game_process.c_str());
                    loading_state = update_loading_state(selected_game_process);
                    additionalText = get_text(loading_state, processId, selected_game_process);
                   
                    refresh_timer = clock();
                }

                if (ImGui::BeginChild(xorstr("LoadingTabChild"), ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMouseInputs))
                {
                    build_manager.render_rotating_dots(additionalText.c_str(), build_manager.center_pos(-20.0f), 26.0f, ImGui::GetTime(), 1.4f, 12, ImVec4(0.50f, 0.50f, 0.50f, 0.76f));

                    ImGui::EndChild();
                }
            }
               
            else
            {
                build_manager.render_large_text(build_manager.program_name.c_str(), build_manager.center_pos(-140));
                build_manager.render_medium_text_disabled(pc_name.get_pc_name().c_str(), build_manager.center_pos(-120));

                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 60);

                ImVec2 tab_bar_start_pos = ImGui::GetCursorPos();

                if (ImGui::BeginTabBar(xorstr("")))
                {
                    if (ImGui::BeginChild(xorstr("EnhancementsChild"), ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysAutoResize))
                    {
                        ImGui::Columns(2, xorstr("EnhancementsColumns"), false);

                        for (size_t i = 0; i < all_game_strings.size(); i += 4)
                        {
                            handle_game_selection(all_game_strings[i], all_game_strings[i + 1], all_game_strings[i + 2], all_game_strings[i + 3]);
                        }

                        ImGui::NextColumn();

                        ImGui::EndChild();                 
                    }
                             
                    ImGui::EndTabBar();
                }
            }

            build_manager.check_window_state(authentication_manager.show_window);

            ImGui::End();
        }
        catch (std::runtime_error& e)
        {

        }
    }

    cheat::CheatManager::LoadingState CheatManager::update_loading_state(const std::string& selected_game_process)
    {
        DWORD processId = build_manager.get_process_id_from_name(selected_game_process.c_str());


        return (processId != 0) ? LoadingState::GameFound : LoadingState::AwaitingLaunch;
    }

    std::string CheatManager::get_text(const LoadingState& loading_state, DWORD processId, const std::string& selected_game_process)
    {
        VMProtectBeginUltra("nidavellir::auth::handle::process");

        std::string result;

        switch (loading_state)
        {
        case LoadingState::AwaitingLaunch:

            auto_inject_dll = false;
            result = selected_game_process + xorstr(" not found. Awaiting game launch.");

            break;

        case LoadingState::GameFound:

            if (clock() - module_scan_refresh_timer > module_scan_refresh_time)
            {
                if (directory.process_memory_usage(processId))
                {
                    std::string saved_file_name_string = flux::variables::get<std::string>(xorstr("saved-file-name"));
                    std::string saved_dll_path = xorstr("C:\\nidavellir\\dll\\") + saved_file_name_string;

                    build_manager.download_a_file(selected_game_file.c_str(), saved_dll_path.c_str());
                    build_manager.inject_dll(saved_dll_path, processId);
                    build_manager.dll_auth_check();

                    decrypt_manager.secure_erase_memory(&saved_file_name_string[0], saved_file_name_string.size());
                    decrypt_manager.secure_erase_memory(&saved_dll_path[0], saved_dll_path.size());

                    if (processId != 0)
                    {
                        result = selected_game_process + xorstr(" found. DLL injection successful.");
                    }
                    else
                    {
                        result = xorstr("Game process found, but injection failed.");
                    }

                    show_window_overlay = false;
                }
                else
                {
                    result = xorstr("Game found, injection successful.");
                }

                module_scan_refresh_timer = clock();
            }
            else
            {
                result = (processId != 0) ? selected_game_process + xorstr(" found. injection pending.") : xorstr("Game found, injection failed.");
            }
            break;

        default:
            result = xorstr("Unknown loading state encountered.");
            break;
        }

        VMProtectEnd();

        return result;
    }
}

