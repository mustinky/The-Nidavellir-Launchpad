#include "notification.h"

namespace notification
{
    NotificationManager::NotificationManager() {}

    void NotificationManager::add_notification(const std::string& message, const ImVec4& color, float duration, ImVec2 window_pos)
    {
        Notification noti;
        noti.message = message;
        noti.color = color;
        noti.duration = duration;
        noti.start_time = ImGui::GetTime();
        noti.window_pos = window_pos; 
        notifications_.push_back(noti);
    }

    void NotificationManager::update_and_draw()
    {
        for (auto it = notifications_.begin(); it != notifications_.end();)
        {
            float current_time = ImGui::GetTime();
            float elapsed_time = current_time - it->start_time;

            if (elapsed_time >= it->duration)
            {
                it = notifications_.erase(it);
            }
            else
            {
                ImVec2 window_size = ImGui::GetWindowSize();
                ImVec2 pos = ImVec2(window_size.x - 10, ImGui::GetCursorPosY());

                ImGui::SetCursorPos(pos);

                ImGui::PushStyleColor(ImGuiCol_Text, it->color);
                ImGui::TextWrapped(xorstr("%s"), it->message.c_str());
                ImGui::PopStyleColor();

                ++it;
            }
        }
    }

    void NotificationManager::log_error(const std::string& message)
    {
        std::ofstream error_log(xorstr("error_log.txt"), std::ios::app);

        if (error_log.is_open())
        {
            error_log << xorstr("[ERROR] ") << message << std::endl;
            error_log.close();
        }       
    }
}
