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
	class input_event
	{
	};


	class keyboard_event : public input_event
	{
	public:
		inline keyboard_event(uint16_t const p_key) : m_key{p_key} {}

		inline uint16_t key() const { return m_key; }
	private:

		uint16_t const m_key;
	};



} //namespace input
