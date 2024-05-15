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
	class input_event_t
	{
	};


	class device_ingress_event : public input_event_t
	{

	private:
		uintptr_t m_id;
		std::u8string_view m_device_name;
	};


	class keyboard_event_t : public input_event_t
	{
	public:
		inline keyboard_event_t(uint16_t const p_key) : m_key{p_key} {}

		inline uint16_t key() const { return m_key; }
	private:

		uint16_t const m_key;
	};



} //namespace input
