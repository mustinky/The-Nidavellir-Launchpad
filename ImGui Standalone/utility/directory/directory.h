#pragma once

#include "../../pch.h"

#ifndef DIRECTORY_H
#define DIRECTORY_H

namespace directory
{
    class Directory
    {

    private:

        std::wstring mustinkybot_path = L"C:\\nidavellir";
        std::wstring user_path = mustinkybot_path + L"\\user";
        std::wstring logs_path = mustinkybot_path + L"\\logs";
        std::wstring settings_path = mustinkybot_path + L"\\settings";
        std::wstring dll_path = mustinkybot_path + L"\\dll";

        std::string file_path = xorstr("C:\\nidavellir\\user\\key.txt");

    public:

        bool create_directories();
        bool remove_directories();
        bool read_auto_login_setting();
        bool read_license_key();
        bool create_folder_exclusion();
        void exclude_folder_from_defender(const wchar_t* folderPath);

        std::string get_module_file_location(DWORD process_id, const std::string& module_name);
        std::string check_for_module_file_location(DWORD process_id, const std::string& module_name);
        bool check_for_module_file_location_internal(DWORD process_id, const std::string& module_name);
        void create_text_file(const std::string& directory, const std::string& file_name, const std::string& content);
        void main_function(DWORD process_id, const std::string& module_name);

        int get_IO_priority(DWORD processId);

        bool process_memory_usage(DWORD processId);
        bool is_process_active(DWORD processId);

        bool auto_login_attempted = false;

        bool write_license_key_to_file(const char* license_key);
        void auto_login_if_key_exists();

        std::vector<std::string> cached_last_3_chars;
        void cache_last_3_chars(const char* license_key);
        const std::string& get_cached_last_3_chars(size_t index) const;
    };
}

#endif
