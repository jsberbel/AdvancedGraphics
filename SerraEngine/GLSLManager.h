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
		static void compileShader(const char* source, const std::string &name, GLuint id);
	public:
		explicit GLSLManager();
		~GLSLManager();
		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
		void compileShadersFromSource(const char* vertexSource, const char* fragmentSource);
		void linkShaders() const;
		void addAttribute(const std::string& attributeName);
		GLint getUniformLocation(const std::string &uniformName) const;
		void bind() const;
		void unbind() const;
		void dispose() const;
	};
}

