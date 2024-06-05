//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include "../keyboard.hpp"

#include <array>

namespace kaos::input::backend
{
	class generic_keyboard: public keyboard
	{
	public:
		inline generic_keyboard(manager& p_manager): keyboard(p_manager){};

		bool init_device_characteristics(raw_device_handle_t p_handle, device_info_t p_deviceInfo) override;
		void handle_input(raw_data_t p_handle, raw_header_t p_header) override;

	private:
		static constexpr uint16_t max_keys = 256;
		uint16_t m_number_of_keys = 0;
		std::array<bool, max_keys> m_pressed_keys;
	};

} //namespace kaos::input::backend
