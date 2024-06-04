#include "ShaderProgram/Program.hpp"

#include <iostream>

namespace OGL
{
	Program::Program(const std::string& folderPath)
	{
		initialize(folderPath + "/vertex.glsl", folderPath + "/fragment.glsl");
	}
	Program::Program(const std::string& vsFilePath, const std::string& fsFilePath)
	{
		initialize(vsFilePath, fsFilePath);
	}
	Program::~Program()
	{
		glDeleteProgram(m_ID);
	}

	void Program::use() const noexcept
	{
		glUseProgram(m_ID);
	}

	void Program::uni1i(const std::string& uniName, int i) const
	{
		glUniform1i(glGetUniformLocation(m_ID, uniName.c_str()), i);
	}
	void Program::uni1f(const std::string& uniName, float f) const
	{
		glUniform1f(glGetUniformLocation(m_ID, uniName.c_str()), f);
	}
	void Program::uni2f(const std::string& uniName, glm::vec2 v) const
	{
		glUniform2f(glGetUniformLocation(m_ID, uniName.c_str()), v.x, v.y);
	}
	void Program::uni3f(const std::string& uniName, glm::vec3 v) const
	{
		glUniform3f(glGetUniformLocation(m_ID, uniName.c_str()), v.x, v.y, v.z);
	}
	void Program::uni4f(const std::string& uniName, glm::vec4 v) const
	{
		glUniform4f(glGetUniformLocation(m_ID, uniName.c_str()), v.x, v.y, v.z, v.w);
	}
	void Program::uniMat4fv(const std::string& uniName, glm::mat4 m) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ID, uniName.c_str()), 1, GL_FALSE, glm::value_ptr(m));
	}
	void Program::uniMVP(const MVP& mvp) const
	{
		uniMat4fv("uModel", mvp.model);
		uniMat4fv("uView", mvp.view);
		uniMat4fv("uProjection", mvp.projection);
	}
	void Program::uniColor(const Color4& color) const
	{
		uni4f("uColor", color);
	}
	void Program::uniColor(const Color3& color) const
	{
		uniColor(Color4(color, 1.0f));
	}

	void Program::initialize(const std::string& vsFilePath, const std::string& fsFilePath)
	{
		m_ID = glCreateProgram();
		unsigned int vs = glCreateShader(GL_VERTEX_SHADER), fs = glCreateShader(GL_FRAGMENT_SHADER);

		std::string vsSource = readFile(vsFilePath), fsSource = readFile(fsFilePath);
		const char* vsCStr = vsSource.c_str(), * fsCStr = fsSource.c_str();
		glShaderSource(vs, 1, &vsCStr, nullptr);
		glShaderSource(fs, 1, &fsCStr, nullptr);

		if (!compileShader(vs))
		{
			std::cout << "Failed to compile Vertex Shader!\n";
			exit(EXIT_FAILURE);
		}
		if (!compileShader(fs))
		{
			std::cout << "Failed to compile Fragment Shader!\n";
			exit(EXIT_FAILURE);
		}

		glAttachShader(m_ID, vs);
		glAttachShader(m_ID, fs);

		if (!linkProgram())
		{
			exit(EXIT_FAILURE);
		}

		glDetachShader(m_ID, vs);
		glDetachShader(m_ID, fs);

		glDeleteShader(vs);
		glDeleteShader(fs);
	}
	bool Program::compileShader(unsigned int shaderID)
	{
		glCompileShader(shaderID);

		int compileStatus;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

		if (compileStatus) return true;

		int length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_alloca(length);
		glGetShaderInfoLog(shaderID, length, nullptr, message);
		std::cout << "Failed to compile shader!\n\tMessage: " << message << '\n';
		return false;
	}
	bool Program::linkProgram()
	{
		glLinkProgram(m_ID);

		int linkStatus;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &linkStatus);

		if (linkStatus) return true;

		int length;
		glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)_alloca(length);
		glGetProgramInfoLog(m_ID, length, nullptr, message);
		std::cout << "Failed to link shader program!\n\tMessage: " << message << '\n';
		return false;
	}
	std::string Program::readFile(const std::string& filepath) const noexcept
	{
		std::ifstream reader(filepath);
		std::string result;
		for (std::string line; std::getline(reader, line); result += '\n')
			result += line;
		if (result.length() == 0)
		{
			std::cout << " File was empty!\n";
		}
		return result;
	}
}