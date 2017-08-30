#pragma once
#ifdef __cplusplus
# include <lua/lua.hpp>
#else
# include <lua/lua.h>
# include <lua/lualib.h>
# include <lua/lauxlib.h>
#endif
#include <vector>
#include <tuple>
#include <vector>
#include "uniform.h"

class VM
{
private:
	lua_State* m_state;
public:
	VM(char* file);
	void Execute(char* function, Uniform uniform);
	std::vector<std::tuple<const char*, const char*, const char*, const char*>> GetGlobals();
	float ResolveField(const char* name);

	lua_State* state() const
	{
		return m_state;
	};
	~VM();
};
