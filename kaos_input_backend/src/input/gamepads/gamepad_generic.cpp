//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#include "gamepad_generic.hpp"
#include "gamepad_generic_p.hpp"

#include <array>
#include <list>
#include <map>
#include <bit>

#include <Windows.h>
#include <hidsdi.h>

#include <CoreLib/Core_Alloca.hpp>
#include <CoreLib/string/core_string_numeric.hpp>

#include <kaos_input_services/logger.hpp>

#include "../HID/HID_data.hpp"
#include "../HID/HID_caps_decoder.hpp"
#include "../handle_maps.hpp"

class RawInputDataPrint: public core::toPrint_base
{
public:
	RawInputDataPrint(DWORD const p_hid_size, DWORD const p_count, BYTE const* const p_data):
		m_hid_size(p_hid_size),
		m_count(p_count),
		m_data(p_data)
	{
	}

	inline uintptr_t size(const char8_t&) const
	{
		if(!m_hid_size || !m_count) return 0;
		const uintptr_t line_size = m_hid_size * 3 + 2;
		return m_count * line_size;
	}

	void get_print(char8_t* p_out) const
	{
		if(!m_hid_size || !m_count) return;

		BYTE const* data_pivot = m_data;
		for(uintptr_t i = 0; i < m_count; ++i)
		{
			*(p_out++) = u8'\n';
			*(p_out++) = u8'\t';

			for(uintptr_t j = 0; j < m_hid_size; ++j)
			{
				core::to_chars_hex_fix(static_cast<uint8_t>(*data_pivot++), std::span<char8_t, 2>{p_out, 2});
				p_out += 2;
				*(p_out++) = u8' ';
			}
		}
	}

private:
	DWORD const m_hid_size;
	DWORD const m_count;
	BYTE const* const m_data;
};


namespace kaos::input::backend
{
	static constexpr uint32_t max_stack_alloc = 65535;
	static uintptr_t g_rawData_offset = offsetof(RAWINPUT, data.hid.bRawData);

	gamepad_generic::~gamepad_generic()
	{
		//if(HID_handle)
		//{
		//	CloseHandle(HID_handle);
		//}
	}

	bool gamepad_generic::init_device_characteristics(raw_device_handle_t p_handle, device_info_t p_deviceInfo)
	{
		m_data_collector_list.clear();
		if(!device::init_device_characteristics(p_handle, p_deviceInfo))
		{
			return false;
		}

#if 0
		{
			HANDLE HID_handle = CreateFileW(
				name().data(),
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
				OPEN_EXISTING, 0, NULL
			);


			std::array<uint8_t, 32> buff;

			memset(buff.data(), 0xFF, buff.size());


			buff[0] = 5;

			DWORD out;
			BOOL res2 = WriteFile(HID_handle, buff.data(), 32, &out, nullptr);

			CloseHandle(HID_handle);
		}
#endif
		const RID_DEVICE_INFO& tinfo = *to_native_handle(p_deviceInfo);

		m_vendor_id  = tinfo.hid.dwVendorId;
		m_product_id = tinfo.hid.dwProductId;
		m_version_n  = tinfo.hid.dwVersionNumber;

		//LOG_INFO("Vendor  "sv, core::toPrint_hex_fix{m_vendor_id});
		//LOG_INFO("Device  "sv, core::toPrint_hex_fix{m_product_id});
		//LOG_INFO("Version "sv, core::toPrint_hex_fix{m_version_n});

		Device_capabilities caps;
		if(!decode_device_capabilities(p_handle, m_preparsedData, caps))
		{
			return false;
		}

		m_inputMaxSize = caps.input_report_size;
		m_input_report_id = caps.input_report_id;

		//---- axis
		m_axis.resize(caps.m_axis_cap.size());
		{
			Axis* current_axis = m_axis.data();
			for(Axis_cap& axis_cap: caps.m_axis_cap)
			{
				current_axis->set_id(axis_cap.usage_id);
				if(axis_cap.byte_size == 1)
				{
					m_data_collector_list.push_back(
						std::make_unique<u8Axis_data_collector>(
							*current_axis,
							axis_cap.link_colection,
							axis_cap.usage_id));
				}
				else if(axis_cap.byte_size == 2)
				{
					m_data_collector_list.push_back(
						std::make_unique<u16Axis_data_collector>(
							*current_axis,
							axis_cap.link_colection,
							axis_cap.usage_id));
				}
				++current_axis;
			}
		}

		//---- buttons
		m_buttons.resize(caps.m_button_array_cap.size);

		{
			uint16_t id = 0;
			for(Button& button: m_buttons)
			{
				button.set_id(id++);
			}
		}

		m_data_collector_list.push_back(
			std::make_unique<Button_data_collector>(
				m_buttons,
				caps.m_button_array_cap.link_colection));

		//---- hat
		m_has_HatSwitch = caps.m_hat_cap.available;
		if(caps.m_hat_cap.available)
		{
			m_data_collector_list.push_back(
				std::make_unique<HatSwitch_data_collector>(
					m_hatSwitch,
					caps.m_hat_cap.link_colection));
		}

		return true;
	}


	NO_INLINE void gamepad_generic::handle_input(raw_data_t p_handle, raw_header_t p_header)
	{
		RAWINPUTHEADER& header = *to_native_handle(p_header);
		UINT size = header.dwSize;
		const uintptr_t data_size = (header.dwSize - g_rawData_offset);
		
		if(m_inputMaxSize > data_size || data_size == 0)
		{
			return;
		}

		void* buffer = core_alloca(size);

		if(GetRawInputData(to_native_handle(p_handle), RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER)) != header.dwSize)
		{
			return;
		}

		RAWINPUT& base_data = *reinterpret_cast<RAWINPUT*>(buffer);
		void* const raw_data = base_data.data.hid.bRawData;

		//LOG_INFO("RawData :"sv, RawInputDataPrint{base_data.data.hid.dwSizeHid, base_data.data.hid.dwCount, base_data.data.hid.bRawData});

		{
			if(base_data.data.hid.dwSizeHid != m_inputMaxSize || base_data.data.hid.dwCount != 1)
			{
				return;
			}

			const uintptr_t size_cross_check = static_cast<uintptr_t>(base_data.data.hid.dwSizeHid) * static_cast<uintptr_t>(base_data.data.hid.dwCount);
			if(data_size != size_cross_check)
			{
				return;
			}
		}

		//LOG_DEBUG("Data align "sv, raw_data);

		uint8_t const* const data_start = reinterpret_cast<const uint8_t*>(raw_data);
		if(*data_start != m_input_report_id)
		{
			return;
		}

		for(collector_proxy_t& collector: m_data_collector_list)
		{
			collector->collect(raw_data, data_size, reinterpret_cast<void*>(m_preparsedData.data()));
		}
	}


	//======== ======== ======== ======== Data Collectors ======== ======== ======== ========

	DataPoint_Collector::~DataPoint_Collector() = default;

	NO_INLINE void Button_data_collector::collect(void* p_data, uintptr_t p_data_size, void* p_preparsed_data)
	{
		PHIDP_PREPARSED_DATA hid_pdata = reinterpret_cast<PHIDP_PREPARSED_DATA>(p_preparsed_data);
		const uintptr_t btn_size = m_buttons.size();
		if(btn_size)
		{
			USAGE* usage_buff = reinterpret_cast<USAGE*>(core_alloca(btn_size * sizeof(USAGE)));
			unsigned long length = static_cast<unsigned long>(btn_size);

			if(HidP_GetUsages(HidP_Input, static_cast<uint16_t>(HID_page::button), m_linked_collection,
				usage_buff, &length, hid_pdata,
				reinterpret_cast<char*>(p_data), static_cast<unsigned long>(p_data_size)) == HIDP_STATUS_SUCCESS)
			{
				if(length > btn_size) return;

				const USAGE* usage_pivot = usage_buff;
				const USAGE* const usage_last = usage_pivot + length;

				for(uintptr_t i = 0; i < btn_size; ++i)
				{
					if(usage_pivot < usage_last)
					{
						if(*usage_pivot == (i + 1))
						{
							++usage_pivot;
							m_buttons[i].set_state(true);
						}
						else
						{
							m_buttons[i].set_state(false);
						}
					}
					else
					{
						m_buttons[i].set_state(false);
					}
				}
			}
		}
	}

	void HatSwitch_data_collector::collect(void* p_data, uintptr_t p_data_size, void* p_preparsed_data)
	{
		unsigned long val = 0;
		if(HidP_GetUsageValue(
			HidP_Input,
			static_cast<USAGE>(HID_page::generic_desktop),
			m_linked_collection,
			static_cast<USAGE>(Usage_generic_desktop::hat_switch),
			&val,
			reinterpret_cast<PHIDP_PREPARSED_DATA>(p_preparsed_data),
			reinterpret_cast<char*>(p_data), static_cast<unsigned long>(p_data_size)
		) == HIDP_STATUS_SUCCESS)
		{

			HatSwitch::State new_state = HatSwitch::State::none;
			switch(val)
			{
			case  0: new_state = HatSwitch::State::up        ; break;
			case  1: new_state = HatSwitch::State::up_right  ; break;
			case  2: new_state = HatSwitch::State::right     ; break;
			case  3: new_state = HatSwitch::State::down_right; break;
			case  4: new_state = HatSwitch::State::down      ; break;
			case  5: new_state = HatSwitch::State::down_left ; break;
			case  6: new_state = HatSwitch::State::left      ; break;
			case  7: new_state = HatSwitch::State::up_left   ; break;
			default: new_state = HatSwitch::State::none      ; break;
			}
			m_hatSwitch.set_value(new_state);
		}
	}

	void u8Axis_data_collector::collect(void* p_data, uintptr_t p_data_size, void* p_preparsed_data)
	{
		unsigned long val = 0;
		if(HidP_GetUsageValue(
			HidP_Input,
			static_cast<USAGE>(HID_page::generic_desktop),
			m_linked_collection,
			static_cast<USAGE>(m_usage),
			&val,
			reinterpret_cast<PHIDP_PREPARSED_DATA>(p_preparsed_data),
			reinterpret_cast<char*>(p_data), static_cast<unsigned long>(p_data_size)
		) == HIDP_STATUS_SUCCESS)
		{
			m_axis.set_value(static_cast<uint16_t>((val << 8) | val));
		}
	}

	void u16Axis_data_collector::collect(void* p_data, uintptr_t p_data_size, void* p_preparsed_data)
	{
		unsigned long val = 0;
		if(HidP_GetUsageValue(
			HidP_Input,
			static_cast<USAGE>(HID_page::generic_desktop),
			m_linked_collection,
			static_cast<USAGE>(m_usage),
			&val,
			reinterpret_cast<PHIDP_PREPARSED_DATA>(p_preparsed_data),
			reinterpret_cast<char*>(p_data), static_cast<unsigned long>(p_data_size)
		) == HIDP_STATUS_SUCCESS)
		{
			m_axis.set_value(static_cast<uint16_t>(val));
		}
	}

} //namespace kaos::input::backend
