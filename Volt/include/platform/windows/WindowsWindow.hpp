#pragma once
#include <string>

#include <platform/windows/VoltWin.hpp>
#include <util/maths/Vector.hpp>
#include <util/Input.hpp>

namespace volt {

	enum class ContextType {
		OPENGL
	};

	class Window  {
		friend class Win32Class;
	private:
		HWND m_handle;
		bool m_is_open;
	public:
		Window(const std::string& title, Vec2 size);

		void update();
		void createContext(ContextType type);
		void makeContextCurrent();

		Vec2 getMousePos() const;
		bool isMouseDown(Mouse button = Mouse::LEFT);

		bool isKeyDown(Keys key);
		bool isKeyJustPressed(Keys key);
		Keys waitForKeyPress();

		void setTitle(const std::string& title);
		void setSize(Vec2 size);
		void setCharCallback(void (*callback)(unsigned char));

		std::string getTitle() const;
		Vec2 getSize() const;
		double getDelta() const;

		void show();
		void hide();

		operator bool() const { return m_is_open; };
	private:
		LRESULT processMessage(HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
	};
}