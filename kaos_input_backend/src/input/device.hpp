//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include "handle_types.hpp"
#include <kaos_input_backend/device_handle.hpp>

#include <kaos_input_backend/backend_structures.hpp>

namespace kaos::input::backend
{
	class manager;

	class device
	{
	public:


		virtual ~device();

	protected:
		inline device(manager& p_manager, DeviceType p_device) : m_this_handle(reinterpret_cast<void*>(this)), m_manager{p_manager}, m_type{p_device}
		{}

		inline manager& get_manager() { return m_manager; }

	public:
		inline DeviceType type() const { return m_type; }
		virtual bool init_device_characteristics(raw_device_handle_t p_handle, device_info_t p_deviceInfo);

		virtual void handle_input(raw_data_t p_handle, raw_header_t p_header) = 0;

		[[nodiscard]] inline raw_device_handle_t raw_device_handle() const { return m_handle; }
		[[nodiscard]] inline std::u8string_view uuid() const { return m_uuid; }

		[[nodiscard]] inline std::wstring_view device_name() const { return m_device_name; }

	public:
		[[nodiscard]] inline device_handle_t device_handle() const { return m_this_handle; }

	private:
		device_handle_t const m_this_handle;
		manager&            m_manager;
		const DeviceType    m_type;
		std::wstring        m_device_name;
		std::u8string       m_uuid;
		raw_device_handle_t m_handle = raw_device_handle_t{nullptr};
	};
} //namespace kaos::input::backend
