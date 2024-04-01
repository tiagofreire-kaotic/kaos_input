//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <memory>
#include <map>

#include <CoreLib/Core_Thread.hpp>
#include <CoreLib/Core_Sync.hpp>

#include "input/handle_types.hpp"
#include "input/device.hpp"

namespace kaos::input
{
	class input_receiver;

	class manager
	{
	public:
		using device_proxy = std::unique_ptr<device>;

	public:
		manager();
		~manager();

		bool startup(input_receiver& p_receiver);
		void shutdown();

		intptr_t dispatch_message(void* p_handle, uint32_t p_message, uintptr_t p_Wparam, intptr_t p_Lparam);

		inline input_receiver& get_receiver() { return *m_receiver; }

	private:
		void run(void* p_receiver);

		void input_device_add   (raw_device_handle_t p_handle);
		void input_device_remove(raw_device_handle_t p_handle);
		void process_input      (raw_data_t      p_handle) const;

		core::thread m_thread;
		core::atomic_spinlock m_spinlock;
		core::event_trap m_trap;

		void* m_underlayer_handle = nullptr;

		input_receiver* m_receiver;
		std::map<raw_device_handle_t, device_proxy> m_deviceList;
	};
} //namespace kaos::input
