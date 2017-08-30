#pragma once
#include <string>

struct Uniform
{
	std::string name;
	float init_value;
	float value;
	float const0;
	std::string const0_name;
	float min;
	float max;

	Uniform(std::string name, float value, float const0, float min, float max, std::string const0_name)
	{
		this->name = name;
		this->value = value;
		this->const0 = const0;
		this->min = min;
		this->max = max;
		this->init_value = value;
		this->const0_name = const0_name;
	}

	Uniform(){}
};
