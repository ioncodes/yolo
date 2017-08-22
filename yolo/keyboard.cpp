#include "keyboard.h"
#include <tuple>

void Keyboard::InstallShortcut(int key0, int key1, KEYBOARDEVENT callback)
{
	m_keys.push_back(std::make_tuple(key0, key1, callback));
}

void Keyboard::CheckKeyboard(GLFWwindow *window)
{
	for(int i = 0; i < m_keys.size(); i++)
	{
		if(glfwGetKey(window, std::get<0>(m_keys[i])) && glfwGetKey(window, std::get<1>(m_keys[i])))
		{
			std::get<2>(m_keys[i])();
		}
	}
}

