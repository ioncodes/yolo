#pragma once
#include <vector>
#include <GLFW/glfw3.h>

typedef void(*KEYBOARDEVENT)();

class Keyboard
{
private:
	std::vector<std::tuple<int, int, KEYBOARDEVENT>> m_keys;
public:
	void InstallShortcut(int key0, int key1, KEYBOARDEVENT callback);
	void CheckKeyboard(GLFWwindow *window);
};
