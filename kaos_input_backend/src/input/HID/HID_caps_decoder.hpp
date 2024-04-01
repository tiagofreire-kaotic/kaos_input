//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <cstdint>
#include <vector>
#include <string_view>

#include <CoreLib/string/core_os_string.hpp>

#include "../device.hpp"
#include "HID_data.hpp"

namespace kaos::input
{

struct Button_array_cap
{
	uint16_t size = 0;
	uint16_t link_colection = 0;
};

struct Axis_cap
{
	Usage_generic_desktop usage_id;
	uint16_t link_colection = 0;
	uint8_t  byte_size;
};

struct Hat_cap
{
	uint16_t link_colection = 0;
	bool     available = false;
};

struct Device_capabilities
{
	Button_array_cap m_button_array_cap {};
	Hat_cap m_hat_cap{};
	std::vector<Axis_cap> m_axis_cap;
	uint16_t input_report_size = 0;
	uint8_t input_report_id = 0;
};

bool decode_device_capabilities(raw_device_handle_t p_handle, std::vector<uint8_t>& p_preparsed_data, Device_capabilities& p_decoded_caps);

}//namespace input
