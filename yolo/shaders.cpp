#include "shaders.h"
#include "filesystem.h"
#include "json.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <lua/lua.h>

Shaders::Shaders()
{
	m_fragmentShaderSource = (char*)DEFAULT_FRAGMENT;
	CompileShader();

	// Set ImGui theme
	ImGuiStyle *style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
	style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
	style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}

Shaders::~Shaders()
{
	glDeleteShader(m_fragmentShader);
	glDeleteShader(m_vertexShader);
	glDeleteProgram(m_program);
	//delete m_vm;
}

void Shaders::LoadFragmentShader()
{
	char *path = Filesystem::RequestFile("frag");
	if (path != NULL)
	{
		char *source = Filesystem::ReadFile(path);
		m_fragmentShaderPath = path;
		m_fragmentShaderSource = source;
		if (shaderLoadedCallback != NULL)
			shaderLoadedCallback();
	}
}

void Shaders::ReloadFragmentShader()
{
	char *source = Filesystem::ReadFile(m_fragmentShaderPath);
	m_fragmentShaderSource = source;
	if (shaderLoadedCallback != NULL)
		shaderLoadedCallback();
}

void Shaders::ReloadUniforms(bool cache)
{
	if (cache)
		CacheUniforms();
	ReloadUniforms();
	if (cache)
		RestoreUniforms();
}

void Shaders::ReloadUniforms()
{
	ResetUniforms();
	m_vm = new VM(m_uniformsPath);
	ParseUniforms();
	if (uniformsLoadedCallback != NULL)
		uniformsLoadedCallback();
}

void Shaders::CacheUniforms()
{
	m_uniformsCache = m_uniforms;
}

void Shaders::RestoreUniforms()
{
	for (int j = 0; j < m_uniforms.size(); j++)
	{
		for (int i = 0; i < m_uniformsCache.size(); i++)
		{
			if(m_uniforms[j].name == m_uniformsCache[i].name)
			{
				m_uniforms[j].value = m_uniformsCache[i].value;
				for(int k = 0; k < m_uniforms[j].constants.size(); k++)
				{
					for (int h = 0; h < m_uniformsCache[i].constants.size(); h++)
					{
						if (std::get<0>(m_uniforms[j].constants[k]) == std::get<0>(m_uniformsCache[i].constants[h]))
						{
							std::get<1>(m_uniforms[j].constants[k]) = std::get<1>(m_uniformsCache[i].constants[h]);
						}
					}
				}
			}
		}
	}
}

void Shaders::LoadUniforms()
{
	char *path = Filesystem::RequestFile("lua");
	if (path != NULL)
	{
		m_uniformsPath = path;
		ResetUniforms();
		m_vm = new VM(path);
		ParseUniforms();
		if (uniformsLoadedCallback != NULL)
			uniformsLoadedCallback();
	}
}

void Shaders::ParseUniforms()
{
	auto globals = m_vm->GetGlobals();
	for(int i = 0; i < globals.size(); i++)
	{
		std::string name = std::get<0>(globals[i]);
		float min = std::get<1>(globals[i]);
		float max = std::get<2>(globals[i]);
		float value = std::get<2>(globals[i]);
		auto constants = std::get<4>(globals[i]);
		m_uniforms.push_back(Uniform(
			name, 
			min,
			max,
			value,
			constants
		));
	}
}

void Shaders::ResetUniforms()
{
	m_uniforms.clear();
}

void Shaders::CompileShader()
{
	// Don't leak stuff
	glDeleteShader(m_fragmentShader);
	glDeleteShader(m_vertexShader);
	glDeleteProgram(m_program);

	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Fullscreen rectangle
	GLfloat vertices[] = {
		-1.0f,  1.0f, 1.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, 1.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create an element array
	GLuint ebo;
	glGenBuffers(1, &ebo);

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	// Create and compile the vertex shader
	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_vertexShader, 1, &VERTEX, NULL);
	glCompileShader(m_vertexShader);

	// Create and compile the fragment shader
	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_fragmentShader, 1, &m_fragmentShaderSource, NULL);
	glCompileShader(m_fragmentShader);

	// Link the vertex and fragment shader into a shader program
	m_program = glCreateProgram();
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);
	glBindFragDataLocation(m_program, 0, "outColor");
	glLinkProgram(m_program);
	glDetachShader(m_program, m_fragmentShader);
	glDetachShader(m_program, m_vertexShader);
	glUseProgram(m_program);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(m_program, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
}

void Shaders::UpdateUniforms()
{
	/*ResetUniforms();
	ParseUniforms();*/
	for (int i = 0; i < m_uniforms.size(); i++)
	{
		Uniform uniform = m_uniforms[i];
		GLint loc = glGetUniformLocation(m_program, uniform.name.data());
		if (loc != -1)
		{
			glUniform1f(loc, m_vm->ResolveTableField(uniform.name.data()));
		}
		for(int j = 0; j < uniform.constants.size(); j++)
		{
			GLint loc = glGetUniformLocation(m_program, std::get<0>(uniform.constants[j]).data());
			if (loc != -1)
			{
				glUniform1f(loc, m_vm->ResolveTableField(std::get<0>(uniform.constants[j]).data()));
			}
		}
		m_vm->Execute("update", uniform);
		uniform.value = m_vm->ResolveTableField(uniform.name.data());
		m_uniforms[i] = uniform;
	}
}

void Shaders::UpdateResolution(float width, float height) const
{
	m_screenWidth = width;
	m_screenHeight = height;
}

void Shaders::UpdateSpectrum(float amplitude) const
{
	GLint loc = glGetUniformLocation(m_program, "spectrum");
	if (loc != -1)
	{
		glUniform1f(loc, amplitude);
	}
	m_spectrum = amplitude;
}

void Shaders::UpdateMouse(float x, float y) const
{
	GLint loc = glGetUniformLocation(m_program, "mouse");
	if (loc != -1)
	{
		glUniform2f(loc, x, y);
	}
}

void Shaders::SetShaderLoadedCallback(SHADERLOADEDPROC callback)
{
	shaderLoadedCallback = callback;
}

void Shaders::SetUniformsLoadedCallback(UNIFORMSLOADEDPROC callback)
{
	uniformsLoadedCallback = callback;
}

void Shaders::DrawUniforms()
{
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_FirstUseEver);
	ImGui::Begin(".: data :.");

	if (ImGui::CollapsingHeader("uniforms", ImGuiTreeNodeFlags_DefaultOpen))
	{
		DrawUniformTools();
	}

	if (ImGui::CollapsingHeader("monitor"))
	{
		DrawUniformMonitor();
	}

	ImGui::End();
}

void Shaders::DrawUniformMonitor()
{
	int total = -1;
	glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &total);
	for (int i = 0; i < total; i++) 
	{
		int name_len = -1, num = -1;
		GLenum type = GL_ZERO;
		char name[100];
		glGetActiveUniform(m_program, GLuint(i), sizeof(name) - 1,
			&name_len, &num, &type, name);
		name[name_len] = 0;
		GLuint location = glGetUniformLocation(m_program, name);
		if(type == GL_FLOAT_VEC2)
		{
			/*GLfloat params;
			glGetnUniformfv(m_program, location, 2, &params);
			printf("%f\n", params);*/
		}
		else if (type == GL_FLOAT)
		{
			GLfloat params;
			glGetUniformfv(m_program, location, &params);
			ImGui::Text(std::string(name).append(std::string(": %f")).data(), params);
		}
		else
		{
			printf("Unknown type %u for %s\n", type, name);
		}
	}
}

void Shaders::DrawUniformTools()
{
	for (int i = 0; i < m_uniforms.size(); i++)
	{
		Uniform uniform = m_uniforms[i];
		auto constants = uniform.constants;
		for (int j = 0; j < constants.size(); j++)
		{
			ImGui::SliderFloat(std::get<0>(constants[j]).data(), &std::get<1>(constants[j]), uniform.min, uniform.max); // implement min max for consts
		}
		uniform.constants = constants;
		m_uniforms[i] = uniform;
	}

	GLint loc = glGetUniformLocation(m_program, "resolution");
	if (loc != -1)
	{
		glUniform2f(loc, m_screenWidth, m_screenHeight);
	}
}

bool Shaders::CheckShaderState(std::vector<GLchar> *errorlog) const
{
	GLint success = 0;
	glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &success);
	GLint maxLength = 0;
	glGetShaderiv(m_fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
	if (!success)
	{
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(m_fragmentShader, maxLength, &maxLength, &errorLog[0]);
		*errorlog = errorLog;
	}
	return success;
}
