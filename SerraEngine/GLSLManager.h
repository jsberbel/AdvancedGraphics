#pragma once
#include <string>
#include <GL\glew.h>

namespace SerraEngine {
	class GLSLManager
	{
		GLuint m_programID;
		GLuint m_vertexShaderID;
		GLuint m_fragmentShaderID;
		int m_numAttributes;
		void compileShader(const std::string &filePath, GLuint id) const;
	public:
		explicit GLSLManager();
		~GLSLManager();
		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
		void linkShaders() const;
		void addAttribute(const std::string& attributeName);
		GLint getUniformLocation(const std::string &uniformName) const;
		void bind() const;
		void unbind() const;
	};
}

