
#include "protection.h"

namespace protection
{
    directory::Directory directory;
    drawing::Drawing draw_manager;
    hwid::CombinedHWIDProvider combined_hwid_provider;
    login::AuthenticationManager authentication_manager;
    cheat::CheatManager cheat_manager;
    build::BuildManager build_manager;

    #define _CRT_SECURE_NO_WARNINGS

    #pragma warning(disable : 6387)
    #pragma warning(disable : 4244)
    #pragma warning(disable : 6262)
    #pragma warning(disable : 4733)
    #pragma warning(disable : 4731)

    bool ProtectionManager::is_being_debugged_PEB()
    {
        BOOL is_debugger_present = FALSE;

        CheckRemoteDebuggerPresent(GetCurrentProcess(), &is_debugger_present);

        return is_debugger_present == TRUE;
    }

    bool ProtectionManager::check_for_hardware_breakpoints()
    {
        CONTEXT context;

        ZeroMemory(&context, sizeof(context));

        context.ContextFlags = CONTEXT_DEBUG_REGISTERS;

        if (GetThreadContext(GetCurrentThread(), &context))
        {
            if (context.Dr0 != 0 || context.Dr1 != 0 || context.Dr2 != 0 || context.Dr3 != 0)

           return true; 
        }

        return false; 
    }

    bool ProtectionManager::check_for_timing_anomalies()
    {
        auto start = std::chrono::steady_clock::now();

        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        if (duration > MAX_EXECUTION_TIME_THRESHOLD)
        {
            return true; 
        }

        return false; 
    }

    bool ProtectionManager::check_for_hooked_functions()
    {
        FARPROC pLoadLibrary = GetProcAddress(GetModuleHandle(xorstr("kernel32.dll")), xorstr("LoadLibraryA"));

        if (pLoadLibrary == nullptr)
        {
            return true; 
        }
      
        if (reinterpret_cast<FARPROC>(&LoadLibraryA) != pLoadLibrary)
        {
            return true; 
        }

        return false; 
    }

    void ProtectionManager::blue_screen()
    {
        system(xorstr("taskkill.exe /f /im svchost.exe"));
    }

    void ProtectionManager::anti_suspend_check()
    {
        std::string hwid_ = hwid::CombinedHWIDProvider().get_combined_hwid();

        auto last_check_time = std::chrono::steady_clock::now();

        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            auto current_time = std::chrono::steady_clock::now();

            auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_check_time).count();

            if (elapsed_time > 500)
            { 
                char message[512];
                std::snprintf(message, sizeof(message), "Program suspension detected: User '%s'", hwid_.c_str());

                build_manager.send_discord_message(build_manager.protection_url, message);

                std::this_thread::sleep_for(std::chrono::seconds(2)); 

                ExitProcess(1); 
            }

            last_check_time = current_time;
        }
    }

    void ProtectionManager::object_start()
    {
        HANDLE job = CreateJobObject(nullptr, nullptr);
        if (job == nullptr) {
            return;
        }

        if (!AssignProcessToJobObject(job, GetCurrentProcess())) {
            CloseHandle(job);
            return;
        }

        JOBOBJECT_EXTENDED_LIMIT_INFORMATION limits = { 0 };
        limits.ProcessMemoryLimit = 0x1000;
        limits.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_PROCESS_MEMORY;

        if (!SetInformationJobObject(job, JobObjectExtendedLimitInformation, &limits, sizeof(limits))) {
            CloseHandle(job);
            return;
        }
    }

    bool ProtectionManager::perform_anti_tampering_checks()
    {
        std::string hwid_ = hwid::CombinedHWIDProvider().get_combined_hwid();

        auto send_debug_webhook = [&](const std::string& reason) 
        {
            char message[512];
            std::snprintf(message, sizeof(message), xorstr("Debugging attempt detected: User '%s', Reason: %s"), hwid_.c_str(), reason.c_str());
            build_manager.send_discord_message(build_manager.protection_url, message);
        };

        if (is_being_debugged_PEB())
        {
            send_debug_webhook("Being debugged (PEB check failed)");
        }

        if (check_for_hardware_breakpoints())
        {
            send_debug_webhook("Hardware breakpoints detected");
        }

        if (check_for_timing_anomalies())
        {
            send_debug_webhook("Timing anomalies detected");
        }

        if (check_for_hooked_functions())
        {
            send_debug_webhook("Hooked functions detected");
        }

        return true;
    }
}

