//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <Windows.h>
#include <hidsdi.h>

namespace kaos::input
{

	void print_HID_caps(PHIDP_PREPARSED_DATA p_data);

} //namespace input
