//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <array>
#include "../mouse.hpp"

namespace kaos::input::backend
{
	class generic_mouse: public mouse
	{
	public:
		generic_mouse(manager& p_manager): mouse(p_manager){};

		bool init_device_characteristics(raw_device_handle_t p_handle, device_info_t p_deviceInfo) override;
		void handle_input(raw_data_t p_handle, raw_header_t p_header) override;

	private:
		static constexpr uint8_t max_buttons   = 5;
		std::array<bool, max_buttons> m_pressed_buttons{};
		uint8_t m_number_of_buttons = 0;
		bool m_has_h_wheel = false;
	};

} //namespace kaos::input::backend
