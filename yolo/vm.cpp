#include "vm.h"
#include <ostream>
#include <tuple>
#include "uniform.h"

VM::VM(char *file)
{
	m_lua = new LuaAdapter(file);
}

void VM::Execute(char* function, Uniform uniform)
{
	m_lua->Set(uniform.const0_name.data(), uniform.const0); // change global constants to user value
	double ret { 0 };
	m_lua->CallFunction(function, ret);
}

std::vector<std::tuple<const char*, const char*, const char*, const char*>> VM::GetGlobals() // basename, const0
{
	std::vector<std::tuple<const char*, const char*, const char*, const char*>> globals;
	globals.push_back(std::make_tuple("time", "time_speed", "time_min", "time_max")); // todo: read from lua globals
	globals.push_back(std::make_tuple("distortion", "distortion", "distortion_min", "distortion_max")); // todo: read from lua globals
	globals.push_back(std::make_tuple("distortion_speed", "distortion_speed", "distortion_speed_min", "distortion_speed_max")); // todo: read from lua globals
	globals.push_back(std::make_tuple("size", "size", "size_min", "size_max")); // todo: read from lua globals
	globals.push_back(std::make_tuple("smooth0", "smooth0", "smooth0_min", "smooth0_max")); // todo: read from lua globals
	globals.push_back(std::make_tuple("smooth1", "smooth1", "smooth1_min", "smooth1_max")); // todo: read from lua globals
	globals.push_back(std::make_tuple("offset_x", "offset_x", "offset_x_min", "offset_x_max")); // todo: read from lua globals
	globals.push_back(std::make_tuple("offset_y", "offset_y", "offset_y_min", "offset_y_max")); // todo: read from lua globals
	globals.push_back(std::make_tuple("edges", "edges", "edges_min", "edges_max")); // todo: read from lua globals
	return globals;
}

float VM::ResolveField(const char *name)
{
	double field = 0.0;
	m_lua->Get(name, field);
	return field;
}

VM::~VM()
{
	delete m_lua;
}
