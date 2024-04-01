//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include "device.hpp"

namespace kaos::input
{
	class mouse: public device
	{
	public:
		inline mouse(manager& p_manager): device(p_manager, DeviceType::mouse){};
	};
} //namespace input
