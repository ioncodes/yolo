#include <imgui/imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <imgui/imgui_internal.h>
#include <stdlib.h>
#include <iostream>
#if _WIN32
#include <Commdlg.h>
#include <Windows.h>
#endif

const int xres = 1280;
const int yres = 720;

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

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

int main()
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
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	if (gl3wInit()) {
		fprintf(stderr, "failed to initialize OpenGL\n");
		return -1;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	ImGui_ImplGlfwGL3_Init(window, true);

	ImVec4 clear_color = ImColor(0.0f, 0.0f, 0.0f, 1.0f);

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
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	float time = 0.0;
	float time_speed = 0.01;
	char chr_xres[32];
	snprintf(chr_xres, sizeof chr_xres, "%f", (float)xres);
	char chr_yres[32];
	snprintf(chr_yres, sizeof chr_yres, "%f", (float)yres);
	bool multisample = false;
	bool smooth = false;


	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// set resolution
		GLint loc = glGetUniformLocation(shaderProgram, "resolution");
		if (loc != -1)
		{
			glUniform2f(loc, (float)strtod(chr_xres, NULL), (float)strtod(chr_yres, NULL));
		}
		// set time
		loc = glGetUniformLocation(shaderProgram, "time");
		if (loc != -1)
		{
			glUniform1f(loc, time);
		}
		if(multisample)
		{
			glEnable(GL_MULTISAMPLE);
		}
		else
		{
			glDisable(GL_MULTISAMPLE);
		}
		if(smooth)
		{
			glEnable(GL_BLEND);
			glEnable(GL_LINE_SMOOTH);
			glEnable(GL_POLYGON_SMOOTH);
		}
		else
		{
			glDisable(GL_BLEND);
			glDisable(GL_LINE_SMOOTH);
			glDisable(GL_POLYGON_SMOOTH);	
		}

		ImGui_ImplGlfwGL3_NewFrame();
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load Fragment", "CTRL+F"))
				{
#if _WIN32
					char filename[MAX_PATH];

					OPENFILENAME ofn;
					ZeroMemory(&filename, sizeof(filename));
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = NULL;
					ofn.lpstrFilter = L"Fragment Shader\0*.frag\0Any File\0*.*\0";
					size_t size = strlen(filename) + 1;
					wchar_t* fragName = new wchar_t[size];
					size_t outSize;
					mbstowcs_s(&outSize, fragName, size, filename, size - 1);
					ofn.lpstrFile = fragName;
					ofn.nMaxFile = MAX_PATH;
					ofn.lpstrTitle = L"Select a fragment shader!";
					ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

					if (GetOpenFileName(&ofn))
					{
						printf("Loading fragment shader '%ls'", fragName);
						char chr_fragName[MAX_PATH];
						size_t i;
						wcstombs_s(&i, chr_fragName, MAX_PATH,
							fragName, MAX_PATH);
						FILE *f = fopen(chr_fragName, "rb");
						fseek(f, 0, SEEK_END);
						long fsize = ftell(f);
						fseek(f, 0, SEEK_SET);  //same as rewind(f);

						char *string = (char*)malloc(fsize + 1);
						fread(string, fsize, 1, f);
						fclose(f);

						string[fsize] = 0;

						glDeleteProgram(shaderProgram);
						fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
						glShaderSource(fragmentShader, 1, &string, NULL);
						glCompileShader(fragmentShader);
						shaderProgram = glCreateProgram();
						glAttachShader(shaderProgram, vertexShader);
						glAttachShader(shaderProgram, fragmentShader);
						glBindFragDataLocation(shaderProgram, 0, "outColor");
						glLinkProgram(shaderProgram);
						glUseProgram(shaderProgram);
					}
#endif
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// split into settings and variables
		ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
		ImGui::Begin(".: settings :.");
		ImGui::SliderFloat("time_speed", &time_speed, 0.0f, 0.1f);
		ImGui::InputText("resolution_x", chr_xres, IM_ARRAYSIZE(chr_xres));
		ImGui::InputText("resolution_y", chr_yres, IM_ARRAYSIZE(chr_yres));
		ImGui::Checkbox("multisample", &multisample);
		ImGui::Checkbox("smooth", &smooth);
		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::End();

		// Rendering
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		ImGui::Render();
		glfwSwapBuffers(window);

		time += time_speed;
	}

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();

	return 0;
}