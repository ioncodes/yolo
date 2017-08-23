#include "menubar.h"
#include <imgui/imgui.h>
#include <algorithm>

void Menubar::InstallMenu(char *name, char *parentName, char *shortcut, MENUEVENT callback)
{
	m_menus.push_back(std::make_tuple(name, parentName, shortcut, callback));
}

void Menubar::DrawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		std::vector<char*> parents;
		for (int j = 0; j < m_menus.size(); j++)
		{
			parents.push_back(std::get<1>(m_menus[j]));
		}
		std::sort(parents.begin(), parents.end());
		auto last = std::unique(parents.begin(), parents.end());
		parents.erase(last, parents.end());

		for (int k = 0; k < parents.size(); k++)
		{
			if (ImGui::BeginMenu(parents[k]))
			{
				for (int i = 0; i < m_menus.size(); i++)
				{
					if (parents[k] == std::get<1>(m_menus[i]))
					{
						char *name = std::get<0>(m_menus[i]);
						char *shortcut = std::get<2>(m_menus[i]);
						MENUEVENT callback = std::get<3>(m_menus[i]);

						if (ImGui::MenuItem(name, shortcut))
						{
							callback();
						}
					}
				}
				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
	}
}
