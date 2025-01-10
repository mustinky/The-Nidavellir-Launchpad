
#pragma once

#include "../../pch.h"

#ifndef HWID_H
#define HWID_H

namespace hwid
{
    class CPUIDProvider
    {

    public:

        static std::string get_cpu_id();
    };

    class MACAddressProvider
    {

    public:

        static std::string get_mac_address();
    };

    class VSProvider
    {

    public:

        static std::string get_volume_serial();
    };

    class DIProvider
    {

    public:

        static std::string get_disk_info();
    };

    class BSProvider
    {

    public:

        static std::string get_bios_serial();
    };

    class IPProvider
    {

    public:

        static std::string get_internal_ip();
        static std::string get_external_ip();
    };

    class PCNameProvider
    {

    public:

        std::string get_pc_name();
    };

    class CombinedHWIDProvider
    {

    public:

        static std::string get_combined_hwid();
    };
}

#endif
