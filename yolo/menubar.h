#pragma once
#include <GLFW/glfw3.h>
#include <tuple>
#include <vector>

typedef void(*MENUEVENT)();

class Menubar
{
private:
	std::vector<std::tuple<char*, char*, char*, MENUEVENT>> m_menus;
	
public:
	void InstallMenu(char *name, char *parentName, char *shortcut, MENUEVENT callback);
	void DrawMenuBar();
};
