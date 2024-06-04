#ifndef _PROGRAM_HPP
#define _PROGRAM_HPP

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <fstream>

struct MVP
{
	glm::mat4 model, view, projection;
	MVP(glm::mat4 projection, glm::mat4 view = glm::mat4(1.0f), glm::mat4 model = glm::mat4(1.0f))
		: model(model), view(view), projection(projection) {}
	MVP() : model(glm::mat4(1.0f)), view(glm::mat4(1.0f)), projection(glm::mat4(1.0f)) {}
};

typedef glm::vec4 Color4;
typedef glm::vec3 Color3;

static Color3 BLACK = Color3(0.0f);
static Color3 WHITE = Color3(1.0f);
static Color3 RED = Color3(1.0f, 0.0f, 0.0f);
static Color3 GREEN = Color3(0.0f, 1.0f, 0.0f);
static Color3 BLUE = Color3(0.0f, 0.0f, 1.0f);
static Color3 PINK = Color3(1.0f, 0.34f, 0.84f);

namespace OGL
{
	class Program
	{
	public:
		Program() : m_ID() {}
		Program(const std::string& folderDirectory);
		Program(const std::string& vsFilePath, const std::string& fsFilePath);
		~Program();
		Program(const Program& other) = delete;

		void use() const noexcept;

		void uni1i(const std::string& uniName, int i) const;
		void uni1f(const std::string& uniName, float f) const;
		void uni2f(const std::string& uniName, glm::vec2 v) const;
		void uni3f(const std::string& uniName, glm::vec3 v) const;
		void uni4f(const std::string& uniName, glm::vec4 v) const;
		void uniMat4fv(const std::string& uniName, glm::mat4 m) const;
		void uniMVP(const MVP& mvp) const;
		void uniColor(const Color4& color) const;
		void uniColor(const Color3& color) const;

		void initialize(const std::string& folder)
		{
			initialize(folder + "/vertex.glsl", folder + "/fragment.glsl");
		}
		void initialize(const std::string& vsFilePath, const std::string& fsFilePath);
	private:
		bool compileShader(unsigned int shaderID);
		bool linkProgram();
		std::string readFile(const std::string& filePath) const noexcept;

		unsigned int m_ID;
	};
}

#endif