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

class keyboard: public device
{
public:
	inline keyboard(manager& p_manager): device(p_manager, DeviceType::keyboard){};
};

} //namespace input
