//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#include "WindowApp.hpp"

#include <Windows.h>
#include <dwmapi.h>
//#include <WinUser.h>

#include <CoreLib/Core_Thread.hpp>
#include <Logger/Logger.hpp>


static constexpr std::wstring_view g_app_class_name = L"KaosEngine";
namespace
{
	class windows_class_auto_unregister
	{
	public:
		inline windows_class_auto_unregister(const std::wstring_view p_class_name, const HINSTANCE p_instance):
			m_class_name(p_class_name),
			m_instance(p_instance)
		{
		}

		inline ~windows_class_auto_unregister()
		{
			UnregisterClassW(m_class_name.data(), m_instance);
		}

	private:
		const std::wstring_view m_class_name;
		const HINSTANCE m_instance;
	};
} //namespace


static LRESULT CALLBACK WndProc(HWND p_handle, UINT p_message, WPARAM p_Wparam, LPARAM p_Lparam)
{
	if(p_handle)
	{
		if(p_message == WM_CREATE)
		{
			CREATESTRUCTW* pCreateStruct = reinterpret_cast<CREATESTRUCTW*>(p_Lparam);
			SetWindowLongPtrW(p_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
		}
		else
		{
			window_app* const app = reinterpret_cast<window_app*>(GetWindowLongPtrW(p_handle, GWLP_USERDATA));
			if(app)
			{
				return app->dispatch_message(p_handle, p_message, p_Wparam, p_Lparam);
			}
		}
	}

	return DefWindowProcW(p_handle, p_message, p_Wparam, p_Lparam);
}

int window_app::run(void* p_instance)
{
	HINSTANCE const instance = reinterpret_cast<HINSTANCE>(p_instance);

	{
		WNDCLASSEXW windowClass;
		windowClass.cbSize         = sizeof(windowClass);
		windowClass.style          = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc    = WndProc;
		windowClass.cbClsExtra     = 0;
		windowClass.cbWndExtra     = 0;
		windowClass.hInstance      = instance;
		windowClass.hIcon          = LoadIconW(instance, L"IDI_ICON1");
		windowClass.hCursor        = nullptr;
		windowClass.hbrBackground  = nullptr; //reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		windowClass.lpszMenuName   = nullptr;
		windowClass.lpszClassName  = g_app_class_name.data();
		windowClass.hIconSm        = nullptr;

		if(!RegisterClassExW(&windowClass))
		{
			LOG_ERROR("Failed to register class"sv);
			return -1;
		}
	}

	windows_class_auto_unregister auto_windowsclass_unregister{g_app_class_name, instance};

	HWND h_window = CreateWindowExW(
		WS_EX_APPWINDOW,
		g_app_class_name.data(), L"",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, //GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2,
		nullptr, nullptr, instance,
		this);

	if(h_window == nullptr)
	{
		LOG_ERROR("Failed to create window"sv);
		return -1;
	}
	{
		DWORD attb = DWMWCP_DONOTROUND;
		DwmSetWindowAttribute(h_window, DWMWA_WINDOW_CORNER_PREFERENCE, &attb, sizeof(attb));
	}
	//m_window_context = m_graphic_API.handler()->create_window_context(h_window);

	//if(!m_window_context.get())
	//{
	//	DestroyWindow(h_window);
	//	LOG_ERROR("Failed to create window context"sv);
	//	return -1;
	//}

	ShowWindow(h_window, SW_SHOW);
	SetForegroundWindow(h_window);
	SetFocus(h_window);

	// Hide the mouse cursor.
	//ShowCursor(false);

	//LONG lStyle = GetWindowLongW(h_window, GWL_STYLE);
	//lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
	//SetWindowLongW(h_window, GWL_STYLE, lStyle);
	//LONG lExStyle = GetWindowLongW(h_window, GWL_EXSTYLE);
	//lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
	//SetWindowLongW(h_window, GWL_EXSTYLE, lExStyle);
	//SetWindowPos(h_window, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);

	m_input_manager.startup();
	int ret_code = 0;
	MSG msg;
	while(!m_should_quit)
	{
		if(PeekMessageW(&msg, 0, 0, 0, TRUE))
		{
			if(msg.message == WM_QUIT /*|| msg.message == WM_NULL*/)
			{
				notify_quit();
				ret_code = static_cast<int>(msg.wParam);
				break;
			}
			//TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
		{
			core::thread_yield();
			//game code
		}
	}

	m_input_manager.shutdown();

	return ret_code;
}

void window_app::notify_quit()
{
	m_should_quit = true;
}

intptr_t window_app::dispatch_message(void* p_handle, uint32_t p_message, uintptr_t p_Wparam, intptr_t p_Lparam)
{
	switch(p_message)
	{
		case WM_PAINT:
			{
				HWND const hwindow = reinterpret_cast<HWND>(p_handle);
				PAINTSTRUCT ps;
				HDC hdc;
				RECT Box;
				GetWindowRgnBox(hwindow, &Box);
				hdc = BeginPaint(hwindow, &ps);
				FillRect(hdc, &Box, (HBRUSH) (COLOR_DESKTOP));
				EndPaint(hwindow, &ps);
			}
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			notify_quit();
			return 0;
		default:
			break;
	}

	return DefWindowProcW(reinterpret_cast<HWND>(p_handle), p_message, p_Wparam, p_Lparam);
}
