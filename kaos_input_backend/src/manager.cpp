//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#include "manager.hpp"

#include <Windows.h>

#include <Logger/Logger.hpp>

#include "input/HID/HID_data.hpp"
#include "input/handle_maps.hpp"

#include "input/device.hpp"
#include "input/mice/mouse_generic.hpp"
#include "input/keyboards/keyboard_generic.hpp"
#include "input/gamepads/gamepad_generic.hpp"



namespace kaos::input
{
	static constexpr std::wstring_view g_plugin_class_name = L"Kaos Input";
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	namespace
	{
		class windows_class_auto_unregister
		{
		public:
			inline windows_class_auto_unregister(const std::wstring_view p_class_name, const HINSTANCE p_instance) :
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

	static manager::device_proxy HID_factory(raw_device_handle_t p_handle, RID_DEVICE_INFO& p_info, manager& p_manager)
	{
		if(HID_page{p_info.hid.usUsagePage} == HID_page::generic_desktop)
		{
			switch(Usage_generic_desktop{p_info.hid.usUsage})
			{
			case Usage_generic_desktop::joystick:
			case Usage_generic_desktop::gamepad:
			{
				manager::device_proxy device = std::make_unique<gamepad_generic>(p_manager);
				if(device->init_device_characteristics(p_handle, to_generic_handle(&p_info)))
				{
					return device;
				}
				else
				{
					//LOG_INFO("Failed to Init"sv);
				}
			}
			break;
			default:
				break;
			}
		}
		//LOG_INFO("Uncaptured"sv);
		return nullptr;
	}

	//======== ======== ======== ======== manager ======== ======== ======== ========
	manager::manager() = default;

	manager::~manager()
	{
		shutdown();
	}

	bool manager::startup(input_receiver& p_receiver)
	{
		if(m_thread.joinable())
		{
			LOG_ERROR("Input service already running"sv);
			return false;
		}

		if(!m_trap.initialized())
		{
			LOG_ERROR("Input initialisation failed"sv);
			return false;
		}

		m_receiver = &p_receiver;


		if(m_thread.create(this, static_cast<void (manager::*)(void*)>(&manager::run), nullptr) != core::thread::Error::None)
		{
			LOG_ERROR("Failed to instantiate Input thread"sv);
			return false;
		}

		return true;
	}

	void manager::shutdown()
	{
		if(m_thread.joinable())
		{
			PostThreadMessageW(m_thread.id(), WM_QUIT, 0, 0);
			m_thread.join();
		}
		m_deviceList.clear();
	}

	void manager::run(void* const)
	{
		HINSTANCE const instance = reinterpret_cast<HINSTANCE>(GetModuleHandleW(nullptr));

		//windows class registration
		{
			WNDCLASSEXW windowClass;
			windowClass.cbSize = sizeof(windowClass);
			windowClass.style = CS_NOCLOSE;
			windowClass.lpfnWndProc = WndProc;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = instance;
			windowClass.hIcon = nullptr;
			windowClass.hCursor = nullptr;
			windowClass.hbrBackground = nullptr;
			windowClass.lpszMenuName = nullptr;
			windowClass.lpszClassName = g_plugin_class_name.data();
			windowClass.hIconSm = 0;

			if(!RegisterClassExW(&windowClass))
			{
				LOG_ERROR("Failed to register Input class"sv);
				return;
			}
		}

		//auxiliary window association
		{
			windows_class_auto_unregister auto_windowsclass_unregister{g_plugin_class_name, instance};
			{
				HWND h_window = CreateWindowExW(
					WS_EX_NOACTIVATE,
					g_plugin_class_name.data(), L"",
					WS_DISABLED,
					0, 0, 0, 0,
					nullptr, nullptr, instance, this);

				if(h_window == nullptr)
				{
					LOG_ERROR("Failed to create Input resource"sv);
					return;
				}
				m_underlayer_handle = h_window;

				//https://docs.microsoft.com/en-us/windows/win32/inputdev/using-raw-input
				{
					const DWORD default_flags = RIDEV_DEVNOTIFY | RIDEV_EXINPUTSINK; //adds device and also ignores legacy messages

					std::array Rid =
					{
						RAWINPUTDEVICE
						{
							.usUsagePage = static_cast<uint16_t>(HID_page::generic_desktop),
							.usUsage = static_cast<uint16_t>(Usage_generic_desktop::mouse),
							.dwFlags = default_flags,
							.hwndTarget = h_window,
					},
					RAWINPUTDEVICE
						{
							.usUsagePage = static_cast<uint16_t>(HID_page::generic_desktop),
							.usUsage = static_cast<uint16_t>(Usage_generic_desktop::keyboard),
							.dwFlags = default_flags,
							.hwndTarget = h_window,
					},
					RAWINPUTDEVICE
						{
							.usUsagePage = static_cast<uint16_t>(HID_page::generic_desktop),
							.usUsage = static_cast<uint16_t>(Usage_generic_desktop::gamepad),
							.dwFlags = default_flags,
							.hwndTarget = h_window,
					},
					RAWINPUTDEVICE
						{
							.usUsagePage = static_cast<uint16_t>(HID_page::generic_desktop),
							.usUsage = static_cast<uint16_t>(Usage_generic_desktop::joystick),
							.dwFlags = default_flags,
							.hwndTarget = h_window,
					},
					};

					if(!RegisterRawInputDevices(Rid.data(), static_cast<UINT>(Rid.size()), sizeof(RAWINPUTDEVICE)))
					{
						DestroyWindow(h_window);
						LOG_ERROR("Failed to register input device"sv);
						return;
					}

				}
			}

			//handle message loop
			{
				MSG msg;
				while(GetMessageW(&msg, 0, 0, 0))
				{
					DispatchMessageW(&msg);
				}
			}

			//cleanup auxiliary window
			{
				HWND h_window = reinterpret_cast<HWND>(m_underlayer_handle);
				m_underlayer_handle = nullptr;
				if(h_window)
				{
					DestroyWindow(h_window);
				}
				else
				{
					//todo
				}
			}
		}
		//todo
	}

	intptr_t manager::dispatch_message(void* p_handle, uint32_t p_message, uintptr_t p_Wparam, intptr_t p_Lparam)
	{
		switch(p_message)
		{
		case WM_DESTROY:
		{
			if(p_handle == m_underlayer_handle)
			{
				m_underlayer_handle = nullptr;
				//todo
			}
		}
		break;
		case WM_INPUT:
			process_input(input::raw_data_t{reinterpret_cast<void*>(p_Lparam)});
			return 0;
		case WM_INPUT_DEVICE_CHANGE:
			if(p_Wparam == GIDC_ARRIVAL)
			{
				input_device_add(input::raw_device_handle_t{reinterpret_cast<void*>(p_Lparam)});
			}
			else if(p_Wparam == GIDC_REMOVAL)
			{
				input_device_remove(input::raw_device_handle_t{reinterpret_cast<void*>(p_Lparam)});
			}
			return 0;
		default:
			break;
		};

		return DefWindowProcW(reinterpret_cast<HWND>(p_handle), p_message, p_Wparam, p_Lparam);
	}


	void manager::input_device_add(raw_device_handle_t p_handle)
	{
		//LOG_INFO("Ingress "sv, p_handle.handle);

		RID_DEVICE_INFO tinfo;
		tinfo.cbSize = sizeof(RID_DEVICE_INFO);
		UINT data_size = sizeof(RID_DEVICE_INFO);
		UINT ret = GetRawInputDeviceInfoW(reinterpret_cast<HANDLE>(p_handle.handle), RIDI_DEVICEINFO, &tinfo, &data_size);

		if(ret == UINT(-1))
		{
			return;
		}

		switch(tinfo.dwType)
		{
		case RIM_TYPEMOUSE:
		{
			device_proxy device = std::make_unique<generic_mouse>(*this);
			if(device->init_device_characteristics(p_handle, to_generic_handle(&tinfo)))
			{
				LOG_INFO("New Mouse "sv, p_handle.handle);
				m_deviceList.insert_or_assign(p_handle, std::move(device));
			}
		}
		break;
		case RIM_TYPEKEYBOARD:
		{
			device_proxy device = std::make_unique<generic_keyboard>(*this);
			if(device->init_device_characteristics(p_handle, to_generic_handle(&tinfo)))
			{
				LOG_INFO("New Keyboard "sv, p_handle.handle);
				m_deviceList.insert_or_assign(p_handle, std::move(device));
			}
		}
		break;
		case RIM_TYPEHID:
		{
			device_proxy device = HID_factory(p_handle, tinfo, *this);
			if(device.get())
			{
				LOG_INFO("New HID "sv, p_handle.handle);
				m_deviceList.insert_or_assign(p_handle, std::move(device));
			}
		}
		break;
		default:
			break;
		}
	}

	void manager::input_device_remove(raw_device_handle_t p_handle)
	{
		LOG_INFO("Digress "sv, p_handle.handle);
		m_deviceList.erase(p_handle);
	}

	void manager::process_input(raw_data_t p_handle) const
	{
		RAWINPUTHEADER header;
		UINT size = sizeof(header);
		UINT ret = GetRawInputData(to_native_handle(p_handle), RID_HEADER, &header, &size, sizeof(RAWINPUTHEADER));

		if(ret == UINT(-1))
		{
			return;
		}

		decltype(m_deviceList)::const_iterator it = m_deviceList.find(to_generic_handle(header.hDevice));

		if(it != m_deviceList.end())
		{
			it->second->handle_input(p_handle, to_generic_handle(&header));
		}
		else if(header.hDevice == nullptr)
		{
			//LOG_INFO("Null-id"sv);
			if(header.dwType == RIM_TYPEMOUSE)
			{
				LOG_INFO("Null-Mouse"sv);
			}
			else if(header.dwType == RIM_TYPEKEYBOARD)
			{
				LOG_INFO("Null-Keyboard"sv);
			}
		}
	}

	//======== ======== ======== ======== WndProc ======== ======== ======== ========

	static LRESULT CALLBACK WndProc(HWND const p_handle, UINT const p_message, WPARAM const p_Wparam, LPARAM const p_Lparam)
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
				manager* const interface = reinterpret_cast<manager*>(GetWindowLongPtrW(p_handle, GWLP_USERDATA));
				if(interface)
				{
					return interface->dispatch_message(p_handle, p_message, p_Wparam, p_Lparam);
				}
			}
		}
		return DefWindowProcW(p_handle, p_message, p_Wparam, p_Lparam);
	}

} // namespace kaos::input
