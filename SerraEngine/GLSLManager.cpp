#include "GLSLManager.h"
#include "ErrorManager.h"
#include <fstream>
#include <vector>

GLSLManager::GLSLManager() : _programID(0), _vertexShaderID(0), _fragmentShaderID(0), _numAttributes(0)
{
	
}

GLSLManager::~GLSLManager()
{
	if (_vertexShaderID != 0) glDeleteShader(_vertexShaderID);
	if (_fragmentShaderID != 0) glDeleteShader(_fragmentShaderID);
	if (_programID != 0) glDeleteProgram(_programID);
}

void GLSLManager::compileShader(const std::string &filePath, GLuint id) {
	std::ifstream shaderFile(filePath);
	if (shaderFile.fail()) {
		perror(filePath.c_str());
		ErrorManager::errorRunTime("Failed to open: " + filePath);
	}
	std::string fileContents = "";
	std::string line;
	while (std::getline(shaderFile, line)) {
		fileContents += line + "\n";
	}
	shaderFile.close();
	const char* contentsPtr = fileContents.c_str();
	glShaderSource(id, 1, &contentsPtr, nullptr);
	glCompileShader(id);
	GLint success = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
		glDeleteShader(id);
		std::cout << &errorLog[0] << std::endl;
		ErrorManager::errorRunTime("Shader " + filePath + " failed to be compiled.");
	}
}

void GLSLManager::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
	_programID = glCreateProgram();
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0) ErrorManager::errorRunTime("Vertex shader failed to be created.");
	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0) ErrorManager::errorRunTime("Fragment shader failed to be created.");
	
	compileShader(vertexShaderFilePath, _vertexShaderID);
	compileShader(fragmentShaderFilePath, _fragmentShaderID);
}

void GLSLManager::linkShaders() {
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);
	glLinkProgram(_programID);

	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);
		glDeleteProgram(_programID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
		std::cout << &errorLog[0] << std::endl;
		ErrorManager::errorRunTime("Program failed to be compiled.");
	}

	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

void GLSLManager::addAttribute(const std::string& attributeName){
	glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
}

GLint GLSLManager::getUniformLocation(const std::string & uniformName){
	GLint location = glGetUniformLocation(_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		ErrorManager::errorRunTime("Uniform " + uniformName + " not found on shader");
		return 0;
	}
	return location;
}

void GLSLManager::bind(){
	glUseProgram(_programID);
	for (int i = 0; i < _numAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}

void GLSLManager::unbind() {
	glUseProgram(0);
	for (int i = 0; i < _numAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}
