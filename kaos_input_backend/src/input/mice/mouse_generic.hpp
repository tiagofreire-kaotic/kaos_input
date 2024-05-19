//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include "../mouse.hpp"

namespace kaos::input::backend
{
	class generic_mouse: public mouse
	{
	public:
		generic_mouse(manager& p_manager): mouse(p_manager){};

		bool init_device_characteristics(raw_device_handle_t p_handle, device_info_t p_deviceInfo) override;
		void handle_input(raw_data_t p_handle, raw_header_t p_header) override;
	};

} //namespace kaos::input::backend
