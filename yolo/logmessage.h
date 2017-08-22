#pragma once
#include <string>
#include "logtype.h"

struct LogMessage
{
	std::string message;
	LogType type;

	LogMessage(std::string message, LogType type);
};
