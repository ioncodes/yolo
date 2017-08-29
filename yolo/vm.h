#pragma once
#ifdef __cplusplus
# include <lua/lua.hpp>
#else
# include <lua/lua.h>
# include <lua/lualib.h>
# include <lua/lauxlib.h>
#endif

class VM
{
private:
	lua_State *m_state;
public:
	VM();
	~VM();
};
