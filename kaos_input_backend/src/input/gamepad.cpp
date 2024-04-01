//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#include "gamepad.hpp"
#include "HID/HID_data.hpp"


#include <Logger/Logger.hpp>

namespace kaos::input
{

	void Button::set_state(bool p_state)
	{
		if(p_state != m_state)
		{
			m_state = p_state;
			//LOG_INFO(m_id, " btn changed "sv, p_state);
		}
	}

	void HatSwitch::set_value(State p_value)
	{
		if(p_value != m_state)
		{
			m_state = p_value;
			//LOG_INFO("HAT changed "sv, static_cast<uint8_t>(p_value));
		}
	}

	void Axis::set_value(uint16_t p_val)
	{
		if(p_val != m_value)
		{
			m_value = p_val;
			//LOG_INFO(static_cast<uint16_t>(m_usage), " Value changed "sv, p_val);
		}
	}

}