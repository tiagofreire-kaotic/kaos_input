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

namespace kaos::input::backend
{
	class input_event_t
	{
	};


	class device_ingress_event_t : public input_event_t
	{
	public:
		DeviceType         device_type;
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
		inline keyboard_event_t(uint16_t const p_key, bool const p_pressed) : m_key{p_key}, m_pressed{p_pressed} {}

		inline uint16_t key() const { return m_key; }
		inline bool pressed() const { return m_pressed; }

	private:

		uint16_t const m_key;
		bool const m_pressed;
	};


	class mouse_move_event_t : public input_event_t
	{
	public:
		inline mouse_move_event_t(int32_t const p_dx, int32_t const p_dy) : m_x {p_dx}, m_y{p_dy} {}

		inline int32_t dx() const { return m_x; }
		inline int32_t dy() const { return m_y; }

	private:
		int32_t const m_x;
		int32_t const m_y;
	};

	class mouse_button_event_t : public input_event_t
	{
	public:

	};


	class mouse_wheel_event_t : public input_event_t
	{
	public:

	};





} //namespace kaos::input::backend
