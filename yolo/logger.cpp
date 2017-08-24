#include "logger.h"
#include <imgui/imgui.h>

std::vector<LogMessage> Logger::m_logs;

void Logger::AddMessage(std::string message, LogType type)
{
	m_logs.push_back(LogMessage(message, type));
}

void Logger::DrawLogWindow(bool draw)
{
	if (draw)
	{
		auto logs = m_logs;
		std::reverse(std::begin(logs), std::end(logs)); // todo: remove this hack, find out how to autoscroll in ImGui
		ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
		ImGui::Begin(".: logs :.");
		for (int i = 0; i < logs.size(); i++)
		{
			if (logs[i].type == LogType::Error)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), logs[i].message.data());
			}
			else if (logs[i].type == LogType::Warning)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), logs[i].message.data());
			}
			else
			{
				ImGui::Text(logs[i].message.data());
			}
		}
		ImGui::End();
	}
}

