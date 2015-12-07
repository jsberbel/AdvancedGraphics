#pragma once
#include <string>
#include <GL\glew.h>

class GLSLManager
{
	GLuint _programID;
	GLuint _vertexShaderID;
	GLuint _fragmentShaderID;
	int _numAttributes;
	void compileShader(const std::string &filePath, GLuint id);
public:
	GLSLManager();
	~GLSLManager();
	void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
	void linkShaders();
	void addAttribute(const std::string& attributeName);
	GLint getUniformLocation(const std::string &uniformName);
	void bind();
	void unbind();
};

