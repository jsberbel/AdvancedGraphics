#include "GLSLManager.h"
#include "ErrorManager.h"
#include <fstream>
#include <vector>
#include <iostream>

namespace SerraEngine {

	GLSLManager::GLSLManager() : 
		m_programID(0), 
		m_vertexShaderID(0), 
		m_fragmentShaderID(0),
		m_numAttributes(0) {}

	GLSLManager::~GLSLManager()
	{
		if (m_vertexShaderID != 0) glDeleteShader(m_vertexShaderID);
		if (m_fragmentShaderID != 0) glDeleteShader(m_fragmentShaderID);
		if (m_programID != 0) glDeleteProgram(m_programID);
	}

	void GLSLManager::compileShader(const std::string &filePath, GLuint id) const
	{
		std::ifstream shaderFile(filePath);
		if (shaderFile.fail()) {
			perror(filePath.c_str());
			fatalError("Failed to open: " + filePath);
		}
		std::string fileContents = "";
		std::string line;
		while (std::getline(shaderFile, line)) {
			fileContents += line + "\n";
		}
		shaderFile.close();
		const auto contentsPtr = fileContents.c_str();
		glShaderSource(id, 1, &contentsPtr, nullptr);
		glCompileShader(id);
		auto success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			auto maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
			glDeleteShader(id);
			std::cout << &errorLog[0] << std::endl;
			fatalError("Shader " + filePath + " failed to be compiled.");
		}
	}

	void GLSLManager::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
		m_programID = glCreateProgram();
		//if (m_programID == 0) fatalError("GLProgram failed to be created.");
		m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (m_vertexShaderID == 0) fatalError("Vertex shader failed to be created.");
		m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (m_fragmentShaderID == 0) fatalError("Fragment shader failed to be created.");

		compileShader(vertexShaderFilePath, m_vertexShaderID);
		compileShader(fragmentShaderFilePath, m_fragmentShaderID);
	}

	void GLSLManager::linkShaders() const
	{
		glAttachShader(m_programID, m_vertexShaderID);
		glAttachShader(m_programID, m_fragmentShaderID);
		glLinkProgram(m_programID);

		auto isLinked = 0;
		glGetProgramiv(m_programID, GL_LINK_STATUS, static_cast<int*>(&isLinked));
		if (isLinked == GL_FALSE) {
			auto maxLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);
			glDeleteProgram(m_programID);
			glDeleteShader(m_vertexShaderID);
			glDeleteShader(m_fragmentShaderID);
			std::cout << &errorLog[0] << std::endl;
			fatalError("Program failed to be compiled.");
		}

		glDetachShader(m_programID, m_vertexShaderID);
		glDetachShader(m_programID, m_fragmentShaderID);
		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);
	}

	void GLSLManager::addAttribute(const std::string& attributeName) {
		glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str());
	}

	GLint GLSLManager::getUniformLocation(const std::string & uniformName) const
	{
		auto location = glGetUniformLocation(m_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + " not found on shader");
			return 0;
		}
		return location;
	}

	void GLSLManager::bind() const
	{
		glUseProgram(m_programID);
		for (auto i = 0; i < m_numAttributes; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLManager::unbind() const
	{
		glUseProgram(0);
		for (auto i = 0; i < m_numAttributes; i++) {
			glDisableVertexAttribArray(i);
		}
	}

}
