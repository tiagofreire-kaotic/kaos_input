//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <cstdint>

namespace kaos::input
{
	enum class DeviceType: uint8_t
	{
		mouse      = 0,
		keyboard   = 1,
		controller = 2,
	};

} //namespace kaos::input

