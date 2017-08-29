#pragma once
#include "uniform.h"
#include "module.h"
#include <neko/neko_vm.h>
#include <vector>

struct Uniform;

class VM
{
private:
	std::vector<Module> m_modules;
public:
	VM();
	~VM();
	void LoadModule(char* name, const char* uniform, const char* function);
	float Execute(Uniform uni);
	value Load(char* file);
};
