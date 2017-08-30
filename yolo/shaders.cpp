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
}

Shaders::~Shaders()
{
	glDeleteShader(m_fragmentShader);
	glDeleteShader(m_vertexShader);
	glDeleteProgram(m_program);
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

void Shaders::LoadUniforms()
{
	char *path = Filesystem::RequestFile("lua");
	if (path != NULL)
	{
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
		const char *base_name = std::get<0>(globals[i]);
		const char *const0 = std::get<1>(globals[i]);
		const char *min = std::get<2>(globals[i]);
		const char *max = std::get<3>(globals[i]);
		m_uniforms.push_back(Uniform(
			base_name, 
			m_vm->ResolveField(base_name),
			m_vm->ResolveField(const0),
			m_vm->ResolveField(min),
			m_vm->ResolveField(max)
		));
	}
}

void Shaders::ResetUniforms()
{
	m_uniforms.clear();
	//delete m_vm;
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
	for (int i = 0; i < m_uniforms.size(); i++)
	{
		Uniform uniform = m_uniforms[i];
		GLint loc = glGetUniformLocation(m_program, uniform.name.data());
		if (loc != -1)
		{
			glUniform1f(loc, uniform.value);
		}
		char function_name[255];
		strcpy(function_name, uniform.name.data());
		std::string new_function_name = std::string(function_name).append("_update").data();
		m_vm->Execute((char*)new_function_name.data(), uniform);
		uniform.value = m_vm->ResolveField(uniform.name.data());
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
	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
	ImGui::Begin(".: uniforms :.");

	for (int i = 0; i < m_uniforms.size(); i++)
	{
		Uniform uniform = m_uniforms[i];
		ImGui::SliderFloat(uniform.name.data(), &uniform.const0, uniform.min, uniform.max);
		m_uniforms[i] = uniform;
	}

	GLint loc = glGetUniformLocation(m_program, "resolution");
	if (loc != -1)
	{
		glUniform2f(loc, m_screenWidth, m_screenHeight);
	}

	ImGui::Text("Spectrum: %f", m_spectrum);

	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate); // todo: get this outta here
	ImGui::End();
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
