//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <string_view>

#include <CoreLib/Core_Type.hpp>

#include "device.hpp"
#include "HID/HID_data.hpp"

namespace kaos::input
{
	class Button
	{
	public:
		inline void set_id(uint16_t p_id)
		{
			m_id = p_id;
		}

		uint16_t id() const
		{
			return m_id;
		}

		inline bool is_pressed() const
		{
			return m_state;
		}

		void set_state(bool p_state);

	private:
		bool m_state = false;
		uint16_t m_id = 0;
	};


	class HatSwitch
	{
	public:
		enum class State: uint8_t
		{
			none		= 0x00,
			up			= 0x01,
			down		= 0x02,
			right		= 0x04,
			left		= 0x08,

			up_right	= up	| right,
			up_left		= up	| left,
			down_right	= down	| right,
			down_left	= down	| left,
			
		};

		inline bool is_pressed() const { return m_state != State::none; }

		inline State state() const { return m_state; }

		void set_value(State p_value);

	private:
		State m_state = State::none;
	};

	class Axis
	{
	public:
		inline void set_id(Usage_generic_desktop p_usage) { m_usage = p_usage; }
		inline Usage_generic_desktop id() const { return m_usage;  }

		void set_value(uint16_t p_val);

		inline uint16_t value() const { return m_value; }
	private:
		uint16_t m_value = 0;
		Usage_generic_desktop m_usage;
	};

	class gamepad: public device
	{
	public:
		inline gamepad(manager& p_manager): device(p_manager, device::DeviceType::controller)
		{
		}
	};


} //namespace input

CORE_MAKE_ENUM_FLAG(kaos::input::HatSwitch::State);
