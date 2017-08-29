#include "vm.h"

VM::VM()
{
	m_state = luaL_newstate();
}

VM::~VM()
{
	lua_close(m_state);
}
