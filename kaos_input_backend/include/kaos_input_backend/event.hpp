//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
/// 
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <cstdint>

#include <string_view>

#include "backend_structures.hpp"
#include "device_handle.hpp"

namespace kaos::input
{
	class input_event_t
	{
	};


	class device_ingress_event_t : public input_event_t
	{
	public:
		DeviceType device_type;
		std::u8string_view device_uuid;
	};

	class device_departure_event_t : public input_event_t
	{
	public:
		std::u8string_view device_uuid;
	};


	class keyboard_event_t : public input_event_t
	{
	public:
		inline keyboard_event_t(uint16_t const p_key) : m_key{p_key} {}

		inline uint16_t key() const { return m_key; }
	private:

		uint16_t const m_key;
	};


	class mouse_move_event_t : public input_event_t
	{
	public:

	};

	class mouse_button_event_t : public input_event_t
	{
	public:

	};


	class mouse_wheel_event_t : public input_event_t
	{
	public:

	};





} //namespace input
