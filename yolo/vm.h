#pragma once
#ifdef __cplusplus
# include <lua/lua.hpp>
#else
# include <lua/lua.h>
# include <lua/lualib.h>
# include <lua/lauxlib.h>
#endif
#include <vector>

class VM
{
private:
	lua_State* m_state;
public:
	VM(char* file);
	float Execute(char* function, float arg0, float arg1, float arg2);
	std::vector<char*> GetGlobals();

	lua_State* state() const
	{
		return m_state;
	};
	~VM();
};
