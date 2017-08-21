#include <imgui/imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <imgui/imgui_internal.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <nativefiledialog/nfd.h>
#include "json.hpp"

int xres = 1280;
int yres = 720;

const char *fragmentSource =
"#version 330 core\n"
"uniform float time;\n"
"uniform vec2 resolution;\n"
"out vec4 outColor;\n"
"void main()\n"
"{\n"
"vec2 r = resolution,\n"
"o = gl_FragCoord.xy - r/2.;\n"
"o = vec2(max(abs(o.x)*.866 + o.y*.5, -o.y)*2. / r.y - .3, atan(o.y,o.x));\n"
"vec4 s = .07*cos(1.5*vec4(0,1,2,3) + time + o.y + time),\n"
"e = s.yzwx,\n"
"f = max(o.x-s,e-o.x);\n"
"outColor = dot(clamp(f*r.y,0.,1.), 72.*(s-e)) * (s-.1) + f;\n"
"}\n";

const char *vertexSource = 
"#version 330 core\n"
"in vec2 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position, 0.0, 1.0);\n"
"}\n";

struct Uniform
{
	std::string name;
	float value;
	float speed;
	float min;
	float max;
	std::string type;
};

using json = nlohmann::json;


GLuint program;
GLuint vertexShader;
GLuint fragmentShader;
std::vector<Uniform> uniforms;
char chr_xres[32];
char chr_yres[32];
char *fragPath;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

static void window_size_callback(GLFWwindow* window, int width, int height)
{
	printf("Resizing to %dx%d\n", width, height);
	xres = width;
	yres = height;
	glfwSetWindowSize(window, width, height);
	glViewport(0, 0, width, height);
}

void init()
{
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
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glBindFragDataLocation(program, 0, "outColor");
	glLinkProgram(program);
	glUseProgram(program);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
}


void update_uniform(Uniform &uniform)
{
	GLint loc = glGetUniformLocation(program, uniform.name.data());
	if (loc != -1)
	{
		glUniform1f(loc, uniform.value);
	}
	if (uniform.type == "+")
		uniform.value += uniform.speed;
	else if (uniform.type == "-")
		uniform.value -= uniform.speed;
	else if (uniform.type == "*")
		uniform.value *= uniform.speed;
	else if (uniform.type == "/")
		uniform.value /= uniform.speed;
	if(uniform.type == "const")
		ImGui::SliderFloat(uniform.name.data(), &uniform.value, uniform.min, uniform.max);
	else
		ImGui::SliderFloat(uniform.name.data(), &uniform.speed, uniform.min, uniform.max);
}

void update_resolution()
{
	GLint loc = glGetUniformLocation(program, "resolution");
	if (loc != -1)
	{
		glUniform2f(loc, (float)strtod(chr_xres, NULL), (float)strtod(chr_yres, NULL));
	}
	ImGui::InputText("resolution_x", chr_xres, IM_ARRAYSIZE(chr_xres));
	ImGui::InputText("resolution_y", chr_yres, IM_ARRAYSIZE(chr_yres));
}

void reload_fragment_shader(char *shader)
{
	glDeleteProgram(program);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &shader, NULL);
	glCompileShader(fragmentShader);
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glBindFragDataLocation(program, 0, "outColor");
	glLinkProgram(program);
	glUseProgram(program);
}

char* load_file(char *extensions)
{
	nfdchar_t *outPath = NULL;
	nfdresult_t result = NFD_OpenDialog(extensions, NULL, &outPath);

	if (result == NFD_OKAY)
	{
		FILE *f;
		fopen_s(&f, outPath, "rb");
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		rewind(f);

		char *string = (char*)malloc(fsize + 1);
		fread(string, fsize, 1, f);
		fclose(f);

		string[fsize] = 0;

		char *extension = strrchr(outPath, '.');

		if(extension && strcmp(extension, ".json"))
			fragPath = (char*)outPath;
		return string;
	}
	return 0; // todo: replace this with something appropriate
}

void load_fragmentshader_from_file()
{
	char *frag = load_file("frag;glsl;glslf");
	reload_fragment_shader(frag);
}

void load_uniforms_from_file()
{
	char *chr_uniforms = load_file("json");
	auto _uniforms = json::parse(chr_uniforms);
	for(int i = 0; i < _uniforms.size(); i++)
	{
		Uniform uniform;
		uniform.name = _uniforms[i]["name"].get<std::string>();
		uniform.value = _uniforms[i]["value"].get<float>();
		uniform.speed = _uniforms[i]["speed"].get<float>();
		uniform.min = _uniforms[i]["min"].get<float>();
		uniform.max = _uniforms[i]["max"].get<float>();
		uniform.type = _uniforms[i]["type"].get<std::string>();
		uniforms.push_back(uniform);
	}
}

int main(int argc, char* argv[])
{
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		return 1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* window = glfwCreateWindow(xres, yres, ".: yolo :.", NULL, NULL);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	if (gl3wInit()) {
		fprintf(stderr, "Failed to initialize OpenGL\n");
		return -1;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported: %s\n", version);

	ImGui_ImplGlfwGL3_Init(window, true);

	init();

	Uniform time;
	time.name = "time";
	time.value = 0.00;
	time.speed = 0.01;
	time.min = 0.00;
	time.max = 0.1;
	time.type = "+";

	uniforms.push_back(time);

	ImVec4 clear_color = ImColor(0.0f, 0.0f, 0.0f, 1.0f);

	snprintf(chr_xres, sizeof chr_xres, "%f", (float)xres);
	snprintf(chr_yres, sizeof chr_yres, "%f", (float)yres);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		ImGui_ImplGlfwGL3_NewFrame();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load Fragment", "CTRL+F"))
				{
					load_fragmentshader_from_file();
				}
				if (ImGui::MenuItem("Load Uniforms", "CTRL+U"))
				{
					load_uniforms_from_file();
				}
				if(ImGui::MenuItem("Reload Fragment", "CTRL+R"))
				{
					FILE *f;
					fopen_s(&f, fragPath, "rb");
					fseek(f, 0, SEEK_END);
					long fsize = ftell(f);
					rewind(f);

					char *string = (char*)malloc(fsize + 1);
					fread(string, fsize, 1, f);
					fclose(f);

					string[fsize] = 0;
					reload_fragment_shader(string);
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
		ImGui::Begin(".: settings :.");

		for (int i = 0; i < uniforms.size(); i++)
		{
			update_uniform(uniforms[i]);
		}

		update_resolution();

		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::End();

		// Rendering
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		ImGui::Render();
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();

	return 0;
}