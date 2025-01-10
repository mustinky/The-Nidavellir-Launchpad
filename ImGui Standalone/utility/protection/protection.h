#pragma once

#include "../../pch.h"

#ifndef PROTECTION_H
#define PROTECTION_H

namespace protection
{
    class ProtectionManager
    {
    private:

        static bool is_being_debugged_PEB();

        static bool check_for_hardware_breakpoints();

        static bool check_for_timing_anomalies();

        static bool check_for_hooked_functions();

        static void blue_screen();

    public:

        static void anti_suspend_check();
        static void object_start();

        static bool perform_anti_tampering_checks();
    };


   
}

#endif
