#include "logmessage.h"

LogMessage::LogMessage(std::string message, LogType type)
{
	this->message = message;
	this->type = type;
}
