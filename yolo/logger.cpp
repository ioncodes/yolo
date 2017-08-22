#include "logger.h"
#include <imgui/imgui.h>

Logger::Logger(bool& draw) : m_draw(draw)
{
	m_draw = draw;
}

void Logger::AddMessage(std::string message, LogType type)
{
	m_logs.push_back(LogMessage(message, type));
}

void Logger::DrawLogWindow()
{
	if (m_draw)
	{
		ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
		ImGui::Begin(".: logs :.");
		for (int i = 0; i < m_logs.size(); i++)
		{
			if (m_logs[i].type == LogType::Error)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), m_logs[i].message.data());
			}
			else if (m_logs[i].type == LogType::Warning)
			{
				ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), m_logs[i].message.data());
			}
			else
			{
				ImGui::Text(m_logs[i].message.data());
			}
		}
		ImGui::End();
	}
}

