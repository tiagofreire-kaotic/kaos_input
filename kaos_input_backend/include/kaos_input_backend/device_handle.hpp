//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
/// 
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <cstdint>

namespace kaos::input::backend
{
	class device;

	class device_handle_t
	{
		friend class device;
	public:
		inline constexpr bool operator < (const device_handle_t& p_2) const { return m_handle < p_2.m_handle; }

		inline uintptr_t id() const { return reinterpret_cast<uintptr_t>(m_handle); }
		inline void* handle() const { return m_handle; }
	private:
		inline device_handle_t(void* p_handle) : m_handle(p_handle) {}
		void* const m_handle;
	};

	//class keyboard_handle_t : public device_handle_t {};
	//class mouse_handle_t : public device_handle_t {};
	//class gamepad_handle_t : public device_handle_t {};
} //namespace kaos::input::backend
