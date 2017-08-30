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
	std::vector<std::tuple<const char*, const char*, const char*, const char*>> GetGlobals();
	float ResolveField(const char* name);
	~VM();
};
