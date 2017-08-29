#include "vm.h"
#include <tuple>

VM::VM()
{
	neko_global_init();
	neko_vm *vm = neko_vm_alloc(NULL);
	neko_vm_select(vm);
}

VM::~VM()
{
	neko_global_free();
}

value VM::Load(char *file) {
	value loader;
	value args[2];
	value exc = NULL;
	value ret;
	loader = neko_default_loader(NULL, 0);
	args[0] = alloc_string(file);
	args[1] = loader;
	ret = val_callEx(loader, val_field(loader, val_id("loadmodule")), args, 2, &exc);
	if (exc != NULL) 
	{
		buffer b = alloc_buffer(NULL);
		val_buffer(b, exc);
		printf("Uncaught exception - %s\n", val_string(buffer_to_string(b)));
		return NULL;
	}
	return ret;
}

void VM::LoadModule(char *name, const char *uniform, const char *function)
{
	const value module = Load(name);
	if (module == NULL)
	{
		printf("Failed to load module !\n");
	}
	else
	{
		m_modules.push_back(Module
		{
			module,
			uniform,
			function
		});
	}
}

float VM::Execute(char *name, float x) 
{
	for(int i = 0; i < m_modules.size(); i++)
	{
		const value module = m_modules[i].module;
		std::string uniform = m_modules[i].uniform;
		std::string function = m_modules[i].function;

		if (std::strcmp(uniform.data(), name) != 0) continue;
		
		value functionField = val_field(module, val_id(function.data()));

		float ret = val_float(val_call1(functionField, alloc_float(x)));

		printf("%s: %f\n", name, ret);

		return ret;
	}
}