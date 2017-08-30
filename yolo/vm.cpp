#include "vm.h"
#include <ostream>
#include <tuple>
#include "uniform.h"

VM::VM(char *file)
{
	m_state = luaL_newstate();
	luaL_openlibs(m_state);
	int result = luaL_loadfile(m_state, file);
	if (result)
	{
		fprintf(stderr, "Error loading script: %s!\n", lua_tostring(m_state, -1));
	}
	if (lua_pcall(m_state, 0, 1, 0))
		fprintf(stderr, "Error loading script: %s!\n", lua_tostring(m_state, -1));
}

void VM::Execute(char* function, Uniform uniform)
{
	// arg1 = const0
	lua_pushnumber(m_state, uniform.const0);
	lua_setglobal(m_state, "time_speed");
	lua_getglobal(m_state, function);
	if (lua_pcall(m_state, 0, 1, 0) != 0)
		printf("Error: %s\n",
			lua_tostring(m_state, -1));
	lua_pop(m_state, 1);
}

std::vector<std::tuple<const char*, const char*, const char*, const char*>> VM::GetGlobals() // basename, const0
{
	std::vector<std::tuple<const char*, const char*, const char*, const char*>> globals;
	globals.push_back(std::make_tuple("time", "time_speed", "time_min", "time_max")); // todo: read from lua globals
	return globals;
}

float VM::ResolveField(const char *name)
{
	lua_getglobal(m_state, name);
	return lua_tonumber(m_state, -1);
}

VM::~VM()
{
	lua_close(m_state);
}
