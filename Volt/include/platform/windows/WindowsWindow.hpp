#pragma once

#ifdef BUILD_WINDOWS

#include <string>
#include <unordered_map>

#include <platform/windows/VoltWin.hpp>
#include <util/maths/Vector.hpp>
#include <util/Input.hpp>

#include <core/GraphicsContext.hpp>

namespace volt {

	class Window  {
		friend class Win32Class;
		friend class WglContext;
	private:
		std::string m_title;
		iVec2 m_size, m_mouse_pos;

		HWND m_handle;

		LARGE_INTEGER m_frame_start;
		double m_delta;

		bool m_is_open;

		Keys m_last_key;
		std::unordered_map<Keys, int> m_keys;
		std::unordered_map<Mouse, int> m_mouse;

		void (*m_char_callback)(unsigned char);

		GraphicsContext* m_context;
	public:
		Window(const std::string& title, iVec2 size);
		~Window();

		void update();
		void createContext(ContextType type);
		void makeContextCurrent();

		iVec2 getMousePos() const;
		bool isMouseDown(Mouse button = Mouse::LEFT);

		bool isKeyDown(Keys key);
		bool isKeyJustPressed(Keys key);
		Keys waitForKeyPress();

		void setTitle(const std::string& title);
		void setSize(iVec2 size);
		void setCharCallback(void (*callback)(unsigned char));

		std::string getTitle() const;
		iVec2 getSize() const;
		double getDelta() const;

		void show();
		void hide();
		void close();

		void setBackgroundColour(Vec4 colour);

		operator bool() const { return m_is_open; };
	private:
		LRESULT processMessage(HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
	};
}

#endif