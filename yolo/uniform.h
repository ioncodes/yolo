#pragma once
#include <string>

struct Uniform
{
	std::string name;
	std::string function;
	std::string path;
	float value;

	Uniform(std::string path, std::string name, std::string function, float value)
	{
		this->path = path;
		this->name = name;
		this->function = function;
		this->value = value;
	}
};
