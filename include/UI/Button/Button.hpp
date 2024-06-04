#ifndef _UI_BUTTON_HPP
#define _UI_BUTTON_HPP

#include "glm/glm.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <string>
#include <iostream>

namespace TTT
{
	class ButtonHandle
	{
	public:
		virtual void onHover(bool hover, glm::ivec2 mousePos, const std::string& tag) = 0;
		virtual void onClick(bool released, glm::ivec2 mousePos, int buttonState, const std::string& tag) = 0;
	};

	class Button
	{
	public:
		Button() : m_BL(), m_TR(), m_Handle(), m_Window(), m_Tag() {}
		Button(glm::ivec2 bl, glm::ivec2 tr, ButtonHandle* handle, GLFWwindow* window, const std::string& tag)
			: m_BL(bl), m_TR(tr), m_Handle(handle), m_Window(window), m_Tag(tag) {}
		void initialize(glm::ivec2 bl, glm::ivec2 tr, ButtonHandle* handle, GLFWwindow* window, const std::string& tag)
		{
			m_BL = bl; m_TR = tr; m_Handle = handle; m_Window = window; m_Tag = tag;
			m_IsInitialized = true;
			glfwGetWindowSize(m_Window, nullptr, &winHeight);
		}

	public:
		void pollEvents();

	private:
		/* Actual Button Stuff */
		bool m_IsInitialized = false;
		int winHeight;
		glm::ivec2 m_BL, m_TR;
		ButtonHandle* m_Handle;
		GLFWwindow* m_Window;
		std::string m_Tag;

		unsigned char m_StateCache = 0x00000000; // last 4 bits = cache. hover, left, middle, right.

	private:
		static unsigned char s_ButtonState;
		static Button* s_Button;
	};
}

#endif
