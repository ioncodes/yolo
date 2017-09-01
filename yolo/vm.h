#pragma once
#include <LuaAdapter/LuaAdapter.hpp>
#include <vector>
#include <tuple>
#include <vector>
#include "uniform.h"

class VM
{
private:
	LuaAdapter* m_lua;
public:
	VM(char* file);
	void Execute(char* function, Uniform uniform);
	std::vector<std::tuple<std::string, float, float, float, std::vector<std::tuple<std::string, float>>>> GetGlobals();
	float ResolveField(const char* name);
	float ResolveTableField(const char* name);
	~VM();
};
