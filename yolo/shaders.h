#pragma once
#include <GL/gl3w.h>
#include "uniform.h"
#include <vector>
#include "vm.h"

typedef void(*SHADERLOADEDPROC)();
typedef void(*UNIFORMSLOADEDPROC)();

class Shaders
{
private:
	GLuint m_fragmentShader;
	GLuint m_vertexShader;
	GLuint m_program;
	char *m_fragmentShaderPath;
	char *m_uniformsPath;
	char *m_fragmentShaderSource;
	const char *VERTEX =
		"#version 330 core\n"
		"in vec2 position;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(position, 0.0, 1.0);\n"
		"}\n";
	const char *DEFAULT_FRAGMENT =
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
	mutable float m_screenWidth;
	mutable float m_screenHeight;
	mutable float m_spectrum;
	VM *m_vm;
	std::vector<Uniform> m_uniforms;
	SHADERLOADEDPROC shaderLoadedCallback; 
	UNIFORMSLOADEDPROC uniformsLoadedCallback;

public:
	Shaders();
	~Shaders();
	void LoadFragmentShader();
	void ReloadFragmentShader();
	void ReloadUniforms();
	void LoadUniforms();
	void ParseUniforms();
	void ResetUniforms();
	void CompileShader();
	void UpdateUniforms();
	void UpdateResolution(float width, float height) const;
	void UpdateSpectrum(float amplitude) const;
	void SetShaderLoadedCallback(SHADERLOADEDPROC callback);
	void SetUniformsLoadedCallback(UNIFORMSLOADEDPROC callback);
	void DrawUniforms();
	bool CheckShaderState(std::vector<GLchar>* errorlog) const;
};
