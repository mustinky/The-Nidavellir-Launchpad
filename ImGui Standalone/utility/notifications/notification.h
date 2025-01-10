
#pragma once

#include "../../pch.h"

#ifndef NOTIFICATION_H
#define NOTIFICATION_H

namespace notification
{
    class NotificationManager
    {
    private:

        struct Notification
        {
            std::string message;
            ImVec4 color;
            float duration;
            float start_time;
            ImVec2 window_pos; 
        };

        std::vector<Notification> notifications_;

    public:

        NotificationManager();

        void add_notification(const std::string& message, const ImVec4& color, float duration, ImVec2 window_pos);

        void update_and_draw();

        void log_error(const std::string& message);
    };
}

#endif
