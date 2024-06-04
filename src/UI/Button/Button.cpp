#include "UI/Button/Button.hpp"
#include "TicTacToe/TicTacToe.hpp"

namespace TTT
{
	void Button::pollEvents()
	{
		if (!m_IsInitialized) return;
		if (glfwGetKey(m_Window, GLFW_KEY_P)) __debugbreak();

		if (s_Button != nullptr && s_Button != this)
		{
			if (s_ButtonState > 0)
			{
				if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && (s_ButtonState & 0b00000100) > 0)
					s_ButtonState ^= 0b00000100;
				if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE && (s_ButtonState & 0b00000010) > 0)
					s_ButtonState ^= 0b00000010;
				if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && (s_ButtonState & 0b00000001) > 0)
					s_ButtonState ^= 0b00000001;
				if ((s_ButtonState & 0b00001000) > 0)
					s_ButtonState ^= 0b00001000;
			}
			if (s_ButtonState == 0)
			{
				s_Button = nullptr;
			}
			else return;
		}

		unsigned char currentState = 0x00000000; // hover, left, middle, right
		double x, y;

		glfwGetCursorPos(m_Window, &x, &y);

		glm::ivec2 mPos = { static_cast<int>(x), winHeight - static_cast<int>(y) };

		if (mPos.x > m_BL.x && mPos.x < m_TR.x && mPos.y > m_BL.y && mPos.y < m_TR.y)
			currentState |= 0b00001000;

		if ((currentState & 0b00001000) > 0)
		{
			if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
				currentState |= 0b00000100;
			if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
				currentState |= 0b00000010;
			if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
				currentState |= 0b00000001;
		}

		if ((currentState & 0b00001000) != (m_StateCache & 0b00001000))
			m_Handle->onHover(currentState & 0b00001000, mPos, m_Tag);
		if ((currentState & 0b00000100) != (m_StateCache & 0b00000100))
			m_Handle->onClick(!(currentState & 0b00000100), mPos, -1, m_Tag);
		if ((currentState & 0b00000010) != (m_StateCache & 0b00000010))
			m_Handle->onClick(!(currentState & 0b00000010), mPos, 0, m_Tag);
		if ((currentState & 0b00000001) != (m_StateCache & 0b00000001))
			m_Handle->onClick(!(currentState & 0b00000001), mPos, 1, m_Tag);

		m_StateCache = currentState;

		if (m_StateCache > 0)
		{
			s_ButtonState = m_StateCache;
			s_Button = this;
		}
	}

	unsigned char Button::s_ButtonState = 0;
	Button* Button::s_Button = nullptr;
}
