#pragma once
#include <string>

struct Uniform
{
	std::string name;
	std::string function;
	std::string path;
	float value;
	float arg1;

	Uniform(std::string path, std::string name, std::string function, float value, float arg1)
	{
		this->path = path;
		this->name = name;
		this->function = function;
		this->value = value;
		this->arg1 = arg1;
	}
};
