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
	for (int i = 0; i < uniform.constants.size(); i++)
	{
		m_lua->Set("__name", uniform.name.data());
		m_lua->Set("__key", std::get<0>(uniform.constants[i]).data());
		m_lua->Set("__value", std::get<1>(uniform.constants[i]));

		double ret;
		m_lua->CallFunction("fix", ret);
	}
	double ret;
	m_lua->CallFunction(function, ret);
}

std::vector<std::tuple<std::string, float, float, float, std::vector<std::tuple<std::string, float>>>> VM::GetGlobals() // remove this huge thing and use structs
{
	std::vector<std::tuple<std::string, float, float, float, std::vector<std::tuple<std::string, float>>>> globals;
	if(m_lua->OpenTable("uniforms"))
	{
		int length;
		m_lua->GetField("size", length);
		for(int i = 0; i < length; i++)
		{
			if (m_lua->OpenNestedTable(std::string("uni").append(std::to_string(i))))
			{
				std::string name;
				m_lua->GetField("name", name);
				float value;
				m_lua->GetField(2, value);
				float min;
				float max;
				
				if (m_lua->OpenNestedTable("range"))
				{
					m_lua->GetField(1, min); // arrays start with 1 here
					m_lua->GetField(2, max);
					m_lua->CloseTable();
				}

				std::vector<std::tuple<std::string, float>> data;
				if (m_lua->OpenNestedTable("data"))
				{
					int size = m_lua->GetTableLength();
					for(int k = 0; k < size; k++)
					{
						std::string const_name;
						m_lua->GetNestedField(k + 1, 1, const_name);
						float init_value;
						m_lua->GetNestedField(k + 1, 2, init_value);
						data.push_back(make_tuple(const_name, init_value));
					}
					globals.push_back(std::make_tuple(name, min, max, value, data));
					m_lua->CloseTable();
				}

				printf("%s\n", name.data());
				printf("%f\n", min);
				printf("%f\n", max);

				for (int u = 0; u < data.size(); u++ )
				{
					printf("%s\n", std::get<0>(data[u]).data());
					printf("%f\n", std::get<1>(data[u]));
				}

				m_lua->CloseTable();
			}
		}
		m_lua->CloseTable();
	}
	return globals;
}

float VM::ResolveField(const char *name)
{
	double field = 0.0;
	m_lua->Get(name, field);
	return field;
}

float VM::ResolveTableField(const char *name)
{
	if (m_lua->OpenTable("uniforms"))
	{
		int length;
		m_lua->GetField("size", length);
		for (int i = 0; i < length; i++)
		{
			if (m_lua->OpenNestedTable(std::string("uni").append(std::to_string(i))))
			{
				if (name[0] != '_')
				{
					std::string _name;
					m_lua->GetField("name", _name);
					if(_name == std::string(name))
					{
						float value;
						m_lua->GetField(2, value);
						m_lua->CloseTable();
						m_lua->CloseTable();
						return value;
					}
				}
				else
				{
					if (m_lua->OpenNestedTable("data"))
					{
						int size = m_lua->GetTableLength();
						for (int k = 0; k < size; k++)
						{
							std::string _name;
							m_lua->GetNestedField(k + 1, 1, _name);
							if (_name == std::string(name))
							{
								float value;
								m_lua->GetNestedField(k + 1, 1, value);
								m_lua->CloseTable();
								m_lua->CloseTable();
								m_lua->CloseTable();
								return value;
							}
						}
						m_lua->CloseTable();
					}
				}
				m_lua->CloseTable();
			}
		}
		m_lua->CloseTable();
	}
}	

VM::~VM()
{
	//delete m_lua;
}
