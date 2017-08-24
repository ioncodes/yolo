#include <imgui/imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <bass/bass.h>
#include "json.hpp"
#include "logmessage.h"
#include "logger.h"
#include "keyboard.h"
#include "music.h"
#include "menubar.h"
#include "shaders.h"

bool drawLogs = false;

bool drawMusic = false;
bool drawSpectrum = false;
bool drawSpectrumPrecise = false;
Music music = Music(drawMusic, drawSpectrum, drawSpectrumPrecise);

Keyboard keyboard = Keyboard();

Menubar menubar = Menubar();

Shaders *shaders;

static void shader_load_callback()
{
	Logger::AddMessage("Shader loaded", Info);
	shaders->CompileShader();
	std::vector<GLchar> shaderLog;
	if (!shaders->CheckShaderState(&shaderLog))
	{
		Logger::AddMessage(&shaderLog[0], Error);
	}
}

static void uniforms_load_callback()
{
	Logger::AddMessage("Uniforms loaded", Info);
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	printf("Resizing to %dx%d\n", width, height);
	glfwSetWindowSize(window, width, height);
	shaders->UpdateResolution(width, height);
	glViewport(0, 0, width, height);
}

int main(int argc, char* argv[])
{
	keyboard.InstallShortcut(GLFW_KEY_LEFT_CONTROL, GLFW_KEY_F, []() -> void { shaders->LoadFragmentShader(); });
	keyboard.InstallShortcut(GLFW_KEY_LEFT_CONTROL, GLFW_KEY_U, []() -> void { shaders->LoadUniforms(); });
	keyboard.InstallShortcut(GLFW_KEY_LEFT_CONTROL, GLFW_KEY_R, []() -> void { shaders->ReloadFragmentShader(); });
	keyboard.InstallShortcut(GLFW_KEY_LEFT_CONTROL, GLFW_KEY_M, []() -> void { music.LoadMusic(); });
	keyboard.InstallShortcut(GLFW_KEY_LEFT_CONTROL, GLFW_KEY_P, []() -> void { drawSpectrumPrecise = !drawSpectrumPrecise; });
	keyboard.InstallShortcut(GLFW_KEY_LEFT_CONTROL, GLFW_KEY_S, []() -> void { music.Stop(); });
	keyboard.InstallShortcut(GLFW_KEY_LEFT_CONTROL, GLFW_KEY_W, []() -> void { music.Pause(); });
	keyboard.InstallShortcut(GLFW_KEY_LEFT_CONTROL, GLFW_KEY_L, []() -> void { music.Play(); });
	keyboard.InstallShortcut(GLFW_KEY_LEFT_CONTROL, GLFW_KEY_G, []() -> void { drawLogs = !drawLogs; });
	keyboard.InstallShortcut(GLFW_KEY_LEFT_CONTROL, GLFW_KEY_T, []() -> void { drawSpectrum = !drawSpectrum; });

	menubar.InstallMenu("Load Fragment", "File", "CTRL+F", []() -> void { shaders->LoadFragmentShader(); });
	menubar.InstallMenu("Load Uniforms", "File", "CTRL+U", []() -> void { shaders->LoadUniforms(); });
	menubar.InstallMenu("Reload Fragment", "File", "CTRL+R", []() -> void { shaders->ReloadFragmentShader(); });
	menubar.InstallMenu("Load Music", "Sound", "CTRL+M", []() -> void { music.LoadMusic(); drawMusic = true; });
	menubar.InstallMenu("Stop Music", "Sound", "CTRL+S", []() -> void { music.Stop(); });
	menubar.InstallMenu("Pause Music", "Sound", "CTRL+W", []() -> void { music.Pause(); });
	menubar.InstallMenu("Play Music", "Sound", "CTRL+L", []() -> void { music.Play(); });
	menubar.InstallMenu("Toggle Spectrum", "Sound", "CTRL+T", []() -> void { drawSpectrum = !drawSpectrum; });
	menubar.InstallMenu("Precise Spectrum", "Sound", "CTRL+P", []() -> void { drawSpectrumPrecise = !drawSpectrumPrecise; });
	menubar.InstallMenu("Toggle Logs", "Dev", "CTRL+G", []() -> void { drawLogs = (bool*)!drawLogs; });

	if (!glfwInit())
		return 1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* window = glfwCreateWindow(1280, 720, ".: yolo :.", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetErrorCallback(error_callback);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	if (gl3wInit()) 
	{
		fprintf(stderr, "Failed to initialize OpenGL\n");
		return -1;
	}

	shaders = new Shaders();
	shaders->SetShaderLoadedCallback(shader_load_callback);
	shaders->SetUniformsLoadedCallback(uniforms_load_callback);
	shaders->UpdateResolution(1280, 720);

	const GLubyte *renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte *version = glGetString(GL_VERSION); // version as a string
	std::string log_render = std::string("Renderer: ").append((char*)renderer);
	std::string log_version = std::string("OpenGL version supported: ").append((char*)version);
	Logger::AddMessage(log_render, LogType::Info);
	Logger::AddMessage(log_version, LogType::Info);

	ImGui_ImplGlfwGL3_Init(window, true);

	ImVec4 clear_color = ImColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		keyboard.CheckKeyboard(window);

		ImGui_ImplGlfwGL3_NewFrame();

		shaders->UpdateSpectrum(music.ReadAmplitude());
		shaders->UpdateUniforms();

		shaders->DrawUniforms();

		menubar.DrawMenuBar();
		music.DrawMusicWindow();
		music.DrawSpectrumWindow();
		Logger::DrawLogWindow(drawLogs);

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
	delete shaders;

	return 0;
}