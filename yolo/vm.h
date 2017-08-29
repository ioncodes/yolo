#pragma once
#include <neko/neko_vm.h>
#include <cstdio>
#include <vector>
#include "module.h"
#include <tuple>

class VM
{
private:
	std::vector<Module> m_modules;
public:
	VM();
	~VM();
	void LoadModule(char* name, const char* uniform, const char* function);
	value Load(char* file);
	float Execute(char* name, float x);
};
