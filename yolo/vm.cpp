#include "vm.h"
#include <ostream>

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

float VM::Execute(char *function, float arg0, float arg1, float arg2)
{
	lua_getglobal(m_state, function);
	/*lua_pushnumber(m_state, arg2);
	lua_pushnumber(m_state, arg1);
	lua_pushnumber(m_state, arg0);*/
	if (lua_pcall(m_state, 0, 1, 0) != 0)
		printf("Error: %s\n",
			lua_tostring(m_state, -1));
	float ret = lua_tonumber(m_state, -1);
	lua_pop(m_state, 1);
	return ret;
}

std::vector<char*> VM::GetGlobals()
{
	std::vector<char*> globals;
	globals.push_back("time"); // todo: read from lua globals
	return globals;
}

VM::~VM()
{
	lua_close(m_state);
}
