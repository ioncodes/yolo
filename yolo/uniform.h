#pragma once
#include <string>

struct Uniform
{
	std::string name;
	float value;
	float speed;
	float min;
	float max;
	std::string type;

	Uniform(std::string name, float value, float speed, float min, float max, std::string type)
	{
		this->name = name;
		this->value = value;
		this->speed = speed;
		this->min = min;
		this->max = max;
		this->type = type;
	}
};
