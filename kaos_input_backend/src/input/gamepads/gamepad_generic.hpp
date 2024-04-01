//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <memory>
#include <vector>
#include <deque>
#include <map>

#include "../gamepad.hpp"

namespace kaos::input
{
	class DataPoint_Collector
	{
	public:
		virtual ~DataPoint_Collector();
		virtual void collect(void* p_data, uintptr_t p_data_size, void* p_preparsed_data) = 0;
	};


	class gamepad_generic: public gamepad
	{
	public:
		inline gamepad_generic(manager& p_manager): gamepad(p_manager){};
		~gamepad_generic();

		bool init_device_characteristics(raw_device_handle_t p_handle, device_info_t p_deviceInfo) override;
		void handle_input(raw_data_t p_handle, raw_header_t p_header) override;

	private:
		uint32_t m_vendor_id  = 0;
		uint32_t m_product_id = 0;
		uint32_t m_version_n  = 0;
		//void* HID_handle = nullptr;

		uintptr_t m_inputMaxSize = 0;
		uint8_t m_input_report_id = 0;

		//----
		std::vector<uint8_t> m_preparsedData;
		std::vector<Button> m_buttons;
		std::vector<Axis>   m_axis;
		HatSwitch m_hatSwitch;
		bool m_has_HatSwitch = false;


		//----
		using collector_proxy_t = std::unique_ptr<DataPoint_Collector>;
		using collector_list_t  = std::vector<collector_proxy_t>;

		collector_list_t m_data_collector_list;
	};
} //namespace input
