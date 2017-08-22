#pragma once
#include <string>
#include <vector>
#include "logmessage.h"


class Logger
{
private:
	std::vector<LogMessage> m_logs;
	bool& m_draw;
public:
	Logger(bool& draw);
	void AddMessage(std::string message, LogType type);
	void DrawLogWindow();
};
