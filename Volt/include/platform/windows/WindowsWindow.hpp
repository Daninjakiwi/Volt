#pragma once

#ifdef BUILD_WINDOWS

#include <string>
#include <unordered_map>

#include <platform/windows/VoltWin.hpp>
#include <util/maths/Vector.hpp>
#include <util/maths/Matrix.hpp>
#include <util/Input.hpp>
#include <render/Mesh.hpp>
#include <render/Material.hpp>
#include <render/Camera.hpp>

#include <core/GraphicsContext.hpp>

namespace volt {

	class Window  {
		friend class Win32Class;
		friend class WglContext;
	private:
		std::string m_title;
		iVec2 m_size, m_mouse_pos, m_mouse_prev;

		HWND m_handle;

		LARGE_INTEGER m_frame_start;
		double m_delta;

		bool m_is_open;
		bool m_cursor_locked;

		Keys m_last_key;
		std::unordered_map<Keys, int> m_keys;
		std::unordered_map<Mouse, int> m_mouse;

		void (*m_char_callback)(unsigned char);
		void (*m_mouse_callback)(volt::iVec2);

		GraphicsContext* m_context;
	public:
		Window(const std::string& title, iVec2 size);
		~Window();

		void update();
		void createContext(ContextType type);
		void makeContextCurrent();

		iVec2 getMousePos() const;
		bool isMouseDown(Mouse button = Mouse::LEFT);
		bool isMouseJustPressed(Mouse button = Mouse::LEFT);

		bool isKeyDown(Keys key);
		bool isKeyJustPressed(Keys key);
		Keys waitForKeyPress();

		void setTitle(const std::string& title);
		void setSize(iVec2 size);
		void setCharCallback(void (*callback)(unsigned char));
		void setMouseCallback(void (*callback)(volt::iVec2));

		std::string getTitle() const;
		iVec2 getSize() const;
		double getDelta() const;

		void show();
		void hide();
		void close();

		void lockCursor();
		void unlockCursor();

		void setBackgroundColour(Vec4 colour);
		void drawQuad(Quad& quad, unsigned int flags = 0);
		void drawTexture(Texture& tex, Vec2 pos, Vec2 size);
		void drawString(const std::string& text, Vec2 pos, unsigned int size, unsigned long long font, Vec4 colour);
		void drawTriangle(Vec2 p1, Vec2 p2, Vec2 p3, Vec4 colour);
		void setViewMatrix(Camera& cam);
		void drawMesh(Mesh& mesh, Material& material, Mat4 transform);
		void loadEnvironmentMap(const std::string& file);

		operator bool() const { return m_is_open; };
	private:
		LRESULT processMessage(HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
	};
}

#endif