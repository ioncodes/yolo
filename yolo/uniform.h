#pragma once
#include <string>

struct Uniform
{
	std::string name;
	float init_value;
	float value;
	float const0;
	float min;
	float max;

	Uniform(std::string name, float value, float const0, float min, float max)
	{
		this->name = name;
		this->value = value;
		this->const0 = const0;
		this->min = min;
		this->max = max;
		this->init_value = value;
	}

	Uniform(){}
};
