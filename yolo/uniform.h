#pragma once
#include <vector>
#include <string>

struct Uniform
{
	std::string name;
	float min;
	float max;
	float value;
	std::vector<std::tuple<std::string, float>> constants;

	// todo: add dynamic constants & reimplement everything :)
	Uniform(std::string name, float min, float max, float value, std::vector<std::tuple<std::string, float>> constants)
	{
		this->name = name;
		this->min = min;
		this->max = max;
		this->value = value;
		this->constants = constants;
	}

	Uniform(){}
};
