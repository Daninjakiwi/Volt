#include <platform/windows/WindowsWindow.hpp>

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

	Window::Window(const std::string& title, Vec2 size) : m_is_open(true)  {

		MONITORINFO monitor = { sizeof(monitor) };
		GetMonitorInfoA(MonitorFromWindow(m_handle, MONITOR_DEFAULTTOPRIMARY), &monitor);

		RECT wr;
		wr.left = 100;
		wr.right = (LONG)size.x + wr.left;
		wr.top = 100;
		wr.bottom = (LONG)size.y + wr.top;
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

	void Window::update() {
		MSG message;
		PeekMessageA(&message, m_handle, 0, 0, PM_REMOVE);
		TranslateMessage(&message);
		DispatchMessageA(&message);

		HDC hdc = GetDC(m_handle);

		SwapBuffers(hdc);
	}

	void Window::createContext(ContextType type) {

	}

	void Window::makeContextCurrent() {

	}

	Vec2 Window::getMousePos() const {
		return {};
	}

	bool Window::isMouseDown(Mouse button) {
		return false;
	}

	bool Window::isKeyDown(Keys key) {
		return false;
	}

	bool Window::isKeyJustPressed(Keys key) {
		return false;
	}

	Keys Window::waitForKeyPress() {
		return Keys::UNDEFINED;
	}

	void Window::setTitle(const std::string& title) {

	}

	void Window::setSize(Vec2 size) {

	}

	void Window::setCharCallback(void (*callback)(unsigned char)) {

	}

	std::string Window::getTitle() const {
		return "";
	}

	Vec2 Window::getSize() const {
		return {};
	}

	double Window::getDelta() const {
		return 0.0;
	}

	void Window::show() {
		ShowWindow(m_handle, SW_SHOW);
	}

	void Window::hide() {
		ShowWindow(m_handle, SW_HIDE);
	}

	LRESULT Window::processMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

		switch (msg)
		{
		case WM_DESTROY:
			m_is_open = false;
			break;
		}
		return DefWindowProcA(hwnd, msg, wparam, lparam);
	}
}