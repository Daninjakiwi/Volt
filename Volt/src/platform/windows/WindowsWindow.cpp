#ifdef BUILD_WINDOWS

#pragma comment (lib, "opengl32.lib")

#include <platform/windows/WindowsWindow.hpp>

#include <platform/windows/WglContext.hpp>

#include <render/stb_image.h>

std::string GetClipboardText() {
	// Try opening the clipboard
	if (!OpenClipboard(nullptr)) {
		return "";
	}

	HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData == nullptr) {
		return "";
	}

	char* pszText = static_cast<char*>(GlobalLock(hData));
	if (pszText == nullptr) {
		return "";
	}

	std::string text(pszText);

	// Release the lock
	GlobalUnlock(hData);

	// Release the clipboard
	CloseClipboard();

	return text;
}

namespace volt {


	class Win32Class {
	private:
		const char* _name;
		HINSTANCE _instance;
	public:
		Win32Class(const char* name) : _name(name), _instance(GetModuleHandleA(nullptr)) {
			WNDCLASSEX class_data = { 0 };
			class_data.cbSize = sizeof(class_data);
			class_data.style = CS_OWNDC;
			class_data.lpfnWndProc = dispatchMessageSetup;
			class_data.cbClsExtra = 0;
			class_data.cbWndExtra = 0;
			class_data.hInstance = nullptr;
			class_data.hIcon = nullptr;
			class_data.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			class_data.lpszMenuName = nullptr;
			class_data.lpszClassName = name;
			class_data.hIconSm = nullptr;
			class_data.hCursor = LoadCursorA(nullptr, IDC_ARROW);

			RegisterClassExA(&class_data);
		}
		~Win32Class() {
			UnregisterClassA(_name, _instance);
		}

		static LRESULT _stdcall dispatchMessageSetup(HWND handle, UINT message, WPARAM w_param, LPARAM l_param) {
			if (message == WM_NCCREATE) {
				const CREATESTRUCTW* const init_data = reinterpret_cast<CREATESTRUCTW*>(l_param);
				Window* const window = static_cast<Window*>(init_data->lpCreateParams);


				SetWindowLongPtrA(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
				SetWindowLongPtrA(handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(dispatchMessage));

				return window->processMessage(handle, message, w_param, l_param);
			}

			return DefWindowProcA(handle, message, w_param, l_param);
		}
		static LRESULT _stdcall dispatchMessage(HWND handle, UINT message, WPARAM w_param, LPARAM l_param) {
			Window* const window = reinterpret_cast<Window*>(GetWindowLongPtrA(handle, GWLP_USERDATA));
			if (message == WM_CREATE) {
				{
					PIXELFORMATDESCRIPTOR pixel_format;
					memset(&pixel_format, 0, sizeof(PIXELFORMATDESCRIPTOR));
					pixel_format.nSize = sizeof(PIXELFORMATDESCRIPTOR);
					pixel_format.nVersion = 1;
					pixel_format.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
					pixel_format.iPixelType = PFD_TYPE_RGBA;
					pixel_format.cColorBits = 32;
					pixel_format.cDepthBits = 32;
					pixel_format.iLayerType = PFD_MAIN_PLANE;

					HDC device = GetDC(handle);

					int nPixelFormat = ChoosePixelFormat(device, &pixel_format);

					SetPixelFormat(device, nPixelFormat, &pixel_format);
				}
			}

			return window->processMessage(handle, message, w_param, l_param);
		}

		const char* getName() {
			return _name;
		}

		HINSTANCE getInstance() {
			return _instance;
		}
	};


	Win32Class window_class("volt_window");

	Window::Window(const std::string& title, iVec2 size) : m_title(title), m_size(size), m_mouse_pos({}), m_mouse_prev({}), m_delta(0.0), m_is_open(true), m_cursor_locked(false), m_last_key(Keys::UNDEFINED), m_char_callback(nullptr), m_mouse_callback(nullptr) {

		MONITORINFO monitor = { sizeof(monitor) };
		GetMonitorInfoA(MonitorFromWindow(m_handle, MONITOR_DEFAULTTOPRIMARY), &monitor);

		RECT wr;
		wr.left = 100;
		wr.right = size.x + wr.left;
		wr.top = 100;
		wr.bottom = size.y + wr.top;
		AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false);

		m_handle = CreateWindowEx(
			0, window_class.getName(),
			title.c_str(),
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
			(monitor.rcMonitor.right - (wr.right - wr.left)) / 2, (monitor.rcMonitor.bottom - (wr.bottom - wr.top)) / 2, wr.right - wr.left, wr.bottom - wr.top,
			nullptr, nullptr, window_class.getInstance(), this
		);

		show();
		
	}

	void Window::setViewMatrix(Camera& cam) {
		if (m_context) {
			m_context->setViewMatrix(cam);
		}
	}

	void Window::drawMesh(Mesh& mesh, Material& material, Mat4 transform) {
		if (m_context) {
			m_context->drawMesh(mesh, material, transform);
		}
	}

	void Window::loadEnvironmentMap(const std::string& file) {
		if (m_context) {
			stbi_set_flip_vertically_on_load(true);
			int width, height, bpp;
			float* data = stbi_loadf(file.c_str(), &width, &height, &bpp, 0);

			m_context->loadEnvironmentMap(data, { width,height });

			stbi_image_free(data);
		}
		
	}

	Window::~Window() {
		if (m_context) {
			delete m_context;
		}
	}

	void Window::update() {
		if (m_context) {
			m_context->renderFrame();
		}



		MSG message;

		while (PeekMessageA(&message, m_handle, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}
		

		HDC hdc = GetDC(m_handle);

		SwapBuffers(hdc);

		LARGE_INTEGER end_time, elapsed;
		LARGE_INTEGER frequency;

		QueryPerformanceFrequency(&frequency);

		QueryPerformanceCounter(&end_time);
		elapsed.QuadPart = end_time.QuadPart - m_frame_start.QuadPart;

		m_frame_start = end_time;

		elapsed.QuadPart *= 100000000;
		elapsed.QuadPart /= frequency.QuadPart;
		m_delta = (double)elapsed.QuadPart / 100000000.0;

		if (m_context) {
			m_context->clear();
		}

		if (m_cursor_locked) {

			POINT p;
			p.x = 640.0f;
			p.y = 360.0f;

			ClientToScreen(m_handle, &p);


			SetCursorPos(p.x, p.y);

		}	
	}

	void Window::createContext(ContextType type) {
		switch (type) {
		case ContextType::OPENGL:
			m_context = new WglContext(this);
			break;
		}
	}

	void Window::makeContextCurrent() {
		if (m_context) {
			m_context->makeCurrent();
		}
	}

	iVec2 Window::getMousePos() const {
		return m_mouse_pos;
	}

	bool Window::isMouseDown(Mouse button) {
		if (m_mouse.find(button) != m_mouse.end()) {
			return true;
		}
		return false;
	}

	bool Window::isMouseJustPressed(Mouse button) {
		auto search = m_mouse.find(button);
		if (search != m_mouse.end() && search->second == 0) {
			search->second = 1;
			return true;
		}
		return false;
	}

	bool Window::isKeyDown(Keys key) {
		if (m_keys.find(key) != m_keys.end()) {
			return true;
		}
		return false;
	}

	bool Window::isKeyJustPressed(Keys key) {
		auto search = m_keys.find(key);
		if (search != m_keys.end() && search->second == 0) {
			search->second = 1;
			return true;
		}
		return false;
	}

	Keys Window::waitForKeyPress() {
		m_last_key = Keys::UNDEFINED;
		while (m_last_key == Keys::UNDEFINED) {
			MSG message;
			PeekMessageA(&message, m_handle, 0, 0, PM_REMOVE);
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}
		return m_last_key;
	}

	void Window::setTitle(const std::string& title) {
		SetWindowTextA(m_handle, title.c_str());
		m_title = title;
	}

	void Window::setSize(iVec2 size) {
		MONITORINFO monitor = { sizeof(monitor) };
		GetMonitorInfoA(MonitorFromWindow(m_handle, MONITOR_DEFAULTTOPRIMARY), &monitor);

		if (size.x == 0 && size.y == 0) {
			SetWindowLongA(m_handle, GWL_STYLE, 0);

			show();

			SetWindowPos(m_handle, HWND_TOP, 0, 0, monitor.rcMonitor.right, monitor.rcMonitor.bottom, 0);

			m_size.x = monitor.rcMonitor.right;
			m_size.y = monitor.rcMonitor.bottom;
		}
		else {
			SetWindowLongA(m_handle, GWL_STYLE, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU);

			show();

			RECT wr;
			GetWindowRect(m_handle, &wr);
			wr.right = size.x + wr.left;
			wr.bottom = size.y + wr.top;
			AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false);

			m_size = size;
			SetWindowPos(m_handle, HWND_TOP, (monitor.rcMonitor.right - (wr.right - wr.left)) / 2, (monitor.rcMonitor.bottom - (wr.bottom - wr.top)) / 2, wr.right - wr.left, wr.bottom - wr.top, 0);
		}
	}

	void Window::setCharCallback(void (*callback)(unsigned char)) {
		m_char_callback = callback;
	}

	void Window::setMouseCallback(void (*callback)(volt::iVec2)) {
		m_mouse_callback = callback;
	}

	std::string Window::getTitle() const {
		return m_title;
	}

	iVec2 Window::getSize() const {
		return m_size;
	}

	double Window::getDelta() const {
		return m_delta;
	}

	void Window::show() {
		ShowWindow(m_handle, SW_SHOW);
	}

	void Window::hide() {
		ShowWindow(m_handle, SW_HIDE);
	}

	void Window::close() {
		m_is_open = false;
	}

	void Window::lockCursor() {
		if (!m_cursor_locked) {
			m_cursor_locked = true;
			ShowCursor(false);
			SetCursor(0);
		}
	
	}

	void Window::unlockCursor() {
		if (m_cursor_locked) {
			m_cursor_locked = false;
			ShowCursor(true);
			SetCursor(LoadCursorA(nullptr, IDC_ARROW));
		}
		
	}

	void Window::setBackgroundColour(Vec4 colour) {
		if (m_context) {
			m_context->setBackgroundColour(colour);
		}
	}

	void Window::drawQuad(Quad& quad, unsigned int flags) {
		if (m_context) {
			m_context->drawQuad(quad, flags);
		}
	}

	void Window::drawTexture(Texture& tex, Vec2 pos, Vec2 size) {
		if (m_context) {
			m_context->drawTexture(tex, pos, size);
		}
	}

	void Window::drawString(const std::string& text, Vec2 pos, unsigned int size, unsigned long long font, Vec4 colour) {
		if (m_context) {
			m_context->drawString(text, pos, size, font, colour);
		}
	}

	void Window::drawTriangle(Vec2 p1, Vec2 p2, Vec2 p3, Vec4 colour) {
		if (m_context) {
			m_context->drawTriangle(p1, p2, p3, colour);
		}
	}

	LRESULT Window::processMessage(HWND handle, UINT message, WPARAM w_param, LPARAM l_param) {
		switch (message)
		{
		case WM_CHAR:
		if (m_char_callback != nullptr) {
			if (w_param != 22) {
				m_char_callback((unsigned char)w_param);
			}
			
 		}
		break;
		case WM_KEYDOWN:


			m_last_key = (Keys)w_param;
			m_keys.emplace((Keys)w_param, 0);

			if (m_last_key == Keys::V && GetAsyncKeyState(VK_CONTROL)) {
				if (m_char_callback != nullptr) {
					for (unsigned char c : GetClipboardText()) {
						m_char_callback(c);
					}
				}
			}

			break;
		case WM_KEYUP:
			m_keys.erase((Keys)w_param);
			break;
		case WM_MOUSEMOVE:
		{

			float x = LOWORD(l_param);
			float y = m_size.y - HIWORD(l_param);



			if (m_cursor_locked) {
				//std::cout << x << std::endl;
				if (x == m_size.x / 2 && y == m_size.y / 2) {
				}
				else {
					m_mouse_pos.x += x - (m_size.x / 2);
					m_mouse_pos.y += ((m_size.y / 2) - HIWORD(l_param));
				}
				
			}
			else {
				m_mouse_pos.x = x;
				m_mouse_pos.y = y;
			}


		}
			break;
		case WM_LBUTTONDOWN:
			m_mouse.emplace(Mouse::LEFT, 0);
			break;
		case WM_LBUTTONUP:
			m_mouse.erase(Mouse::LEFT);
			break;
		case WM_RBUTTONDOWN:
			m_mouse.emplace(Mouse::RIGHT, 0);
			break;
		case WM_RBUTTONUP:
			m_mouse.erase(Mouse::RIGHT);
			break;
		case WM_DESTROY:
			m_is_open = false;
			break;
		}
		return DefWindowProcA(handle, message, w_param, l_param);
	}
}
#endif