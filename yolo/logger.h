#pragma once
#include <string>
#include <vector>
#include "logmessage.h"


class Logger
{
private:
	static std::vector<LogMessage> m_logs;
public:
	static void AddMessage(std::string message, LogType type);
	static void DrawLogWindow(bool draw);
};
