//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#include "HID_caps_decoder.hpp"

#include <vector>
#include <span>
#include <set>
#include <utility>
#include <bit>
#include <Windows.h>
#include <hidsdi.h>

#include <CoreLib/Core_Alloca.hpp>
#include <CoreLib/Core_extra_compiler.hpp>

#include <kaos_input_services/logger.hpp>

#include "HID_data.hpp"

#include "debug/HID_cap_print.hpp"

#if 0
namespace kaos::input
{
	struct HID_base_caps
	{
		uint16_t	UsagePage;
		uint8_t		ReportID;
		uint8_t		IsAlias;

		uint16_t	BitField;
		uint16_t	LinkCollection;

		uint16_t	LinkUsage;
		uint16_t	LinkUsagePage;

		uint8_t		IsRange;
		uint8_t		IsStringRange;
		uint8_t		IsDesignatorRange;
		uint8_t		IsAbsolute;

		uint32_t	Reserved[10];

		union {
			struct {
				uint16_t	UsageMin,			UsageMax;
				uint16_t	StringMin,			StringMax;
				uint16_t	DesignatorMin,		DesignatorMax;
				uint16_t	DataIndexMin,		DataIndexMax;
			} Range;
			struct  {
				uint16_t	Usage,				Reserved1;
				uint16_t	StringIndex,		Reserved2;
				uint16_t	DesignatorIndex,	Reserved3;
				uint16_t	DataIndex,			Reserved4;
			} NotRange;
		};
	};
} //namespace input
#define STATIC_ASSERT_DATA_COMPATRIBLE(T1, D1, T2, D2) \
	static_assert(offsetof(T1, D1) == offsetof(T2, D2)); \
	static_assert(sizeof(decltype(std::declval<T1>().D1)) == sizeof(decltype(std::declval<T2>().D2))); \

#define STATIC_ASSERT_DATA_COMPATRIBLE_EQ(T1, T2, D) STATIC_ASSERT_DATA_COMPATRIBLE(T1, D, T2, D)

#define STATIC_ASSERT_DOUBLE_DATA_COMPATRIBLE_EQ(T1, T2, T3, D) \
	STATIC_ASSERT_DATA_COMPATRIBLE_EQ(T1, T2, D) \
	STATIC_ASSERT_DATA_COMPATRIBLE_EQ(T1, T3, D)

#define STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(D)	STATIC_ASSERT_DOUBLE_DATA_COMPATRIBLE_EQ(input::HID_base_caps, HIDP_BUTTON_CAPS, HIDP_VALUE_CAPS, D)
#define STATIC_ASSERT_HID_BUTTON_DATA_COMPATIBLE(D)	STATIC_ASSERT_DATA_COMPATRIBLE_EQ(input::HID_button_caps, HIDP_BUTTON_CAPS, D)
#define STATIC_ASSERT_HID_VALUE_DATA_COMPATIBLE(D)	STATIC_ASSERT_DATA_COMPATRIBLE_EQ(input::HID_value_caps, HIDP_VALUE_CAPS, D)

static_assert(alignof(::input::HID_base_caps) == alignof(HIDP_BUTTON_CAPS));
static_assert(alignof(::input::HID_base_caps) == alignof(HIDP_VALUE_CAPS));
static_assert(sizeof(::input::HID_base_caps) == sizeof(HIDP_BUTTON_CAPS));
static_assert(sizeof(::input::HID_base_caps) == sizeof(HIDP_VALUE_CAPS));
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(UsagePage);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(ReportID);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(IsAlias);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(BitField);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(LinkCollection);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(LinkUsage);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(LinkUsagePage);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(IsRange);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(IsStringRange);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(IsDesignatorRange);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(Range);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(Range.UsageMin);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(Range.UsageMax);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(Range.StringMin);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(Range.StringMax);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(Range.DesignatorMin);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(Range.DesignatorMax);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(Range.DataIndexMin);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(Range.DataIndexMax);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(NotRange);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(NotRange.Usage);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(NotRange.StringIndex);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(NotRange.DesignatorIndex);
STATIC_ASSERT_HID_BASE_DATA_COMPATIBLE(NotRange.DataIndex);
#endif

namespace kaos::input
{
	NO_INLINE bool decode_device_capabilities(raw_device_handle_t const p_handle, std::vector<uint8_t>& p_preparsed_data, Device_capabilities& p_decoded_caps)
	{
		p_decoded_caps = Device_capabilities{};
		HANDLE thandle = reinterpret_cast<HANDLE>(p_handle.handle);

		UINT preparsed_data_size = 0;
		UINT ret = GetRawInputDeviceInfoW(thandle, RIDI_PREPARSEDDATA, nullptr, &preparsed_data_size);

		if(ret == UINT(-1) || preparsed_data_size == 0)
		{
			LOG_ERROR();
			return false;
		}

		if(preparsed_data_size > std::numeric_limits<uint16_t>::max())
		{
			LOG_ERROR();
			return false;
		}

		p_preparsed_data.resize(preparsed_data_size);

		PHIDP_PREPARSED_DATA hid_pdata = reinterpret_cast<PHIDP_PREPARSED_DATA>(p_preparsed_data.data());
		ret = GetRawInputDeviceInfoW(thandle, RIDI_PREPARSEDDATA, hid_pdata, &preparsed_data_size);
		
		if(ret != p_preparsed_data.size())
		{
			LOG_ERROR();
			return false;
		}


		//print_HID_caps(hid_pdata);


		HIDP_CAPS capabilities;
		if(HidP_GetCaps(hid_pdata, &capabilities) != HIDP_STATUS_SUCCESS)
		{
			LOG_ERROR();
			return false;
		}

		const uint16_t report_lenght = capabilities.InputReportByteLength;

		if(report_lenght == 0)
		{
			LOG_ERROR();
			return false;
		}

		if(capabilities.NumberInputButtonCaps == 0 && capabilities.NumberInputValueCaps == 0)
		{
			LOG_DEBUG();
			return false;
		}


		std::vector<HIDP_BUTTON_CAPS> button_caps_data;
		std::vector<HIDP_VALUE_CAPS> value_caps_data;

		if(capabilities.NumberInputButtonCaps)
		{
			button_caps_data.resize(capabilities.NumberInputButtonCaps);
			uint16_t retrieved_button_caps = capabilities.NumberInputButtonCaps;
			if(
				HidP_GetButtonCaps(HidP_Input, button_caps_data.data(), &retrieved_button_caps, hid_pdata) != HIDP_STATUS_SUCCESS ||
				retrieved_button_caps != capabilities.NumberInputButtonCaps
				)
			{
				LOG_DEBUG("Failed to get input button caps for device "sv, p_handle.handle);
				return false;
			}
		}
		if(capabilities.NumberInputValueCaps)
		{
			value_caps_data.resize(capabilities.NumberInputValueCaps);

			uint16_t retrieved_value_caps = capabilities.NumberInputValueCaps;
			if(
				HidP_GetValueCaps(HidP_Input, value_caps_data.data(), &retrieved_value_caps, hid_pdata) != HIDP_STATUS_SUCCESS ||
				retrieved_value_caps != capabilities.NumberInputValueCaps
				)
			{
				LOG_DEBUG("Failed to get input value caps for device "sv, p_handle.handle);
				return false;
			}
		}

		uint8_t input_report_id = 0;
		if(!button_caps_data.empty())
		{
			input_report_id = button_caps_data.front().ReportID;
		}
		else if(!value_caps_data.empty())
		{
			input_report_id = value_caps_data.front().ReportID;
		}

		Button_array_cap out_button_caps;
		Hat_cap out_hat_caps;
		std::vector<Axis_cap> out_axis_caps;
		std::set<uint16_t> axis_usage_set;

		for(HIDP_BUTTON_CAPS const& cap: button_caps_data)
		{
			if(cap.IsRange)
			{
				const uint16_t size = cap.Range.DataIndexMax - cap.Range.DataIndexMin + 1;
				if(
					HID_page{cap.LinkUsagePage} == HID_page::generic_desktop &&
					HID_page{cap.UsagePage} == HID_page::button &&
					cap.IsAbsolute)
				{
					if(
						cap.Range.UsageMin == 0 ||
						cap.Range.UsageMin > cap.Range.UsageMax)
					{
						LOG_ERROR();
						return false;
					}

					const uint16_t usage_size = cap.Range.UsageMax - cap.Range.UsageMin + 1;

					if(size != usage_size)
					{
						LOG_ERROR();
						return false;
					}

					if(cap.Range.UsageMin == 1)
					{
						if(!out_button_caps.size)
						{
							out_button_caps.size = size;
							out_button_caps.link_colection = cap.LinkCollection;
						}
						else
						{
							LOG_DEBUG("Found Multiple button caps"sv);
						}
					}
				}
			}
		}

		for(HIDP_VALUE_CAPS& cap: value_caps_data)
		{
			if(
				HID_page{cap.LinkUsagePage} == HID_page::generic_desktop &&
				HID_page{cap.UsagePage} == HID_page::generic_desktop &&
				cap.IsAbsolute)
			{
				uint16_t const bit_size = cap.BitSize;
				Usage_generic_desktop const  usage = Usage_generic_desktop{cap.NotRange.Usage};

				switch(usage)
				{
				case Usage_generic_desktop::X:
				case Usage_generic_desktop::Y:
				case Usage_generic_desktop::Z:
				case Usage_generic_desktop::Rx:
				case Usage_generic_desktop::Ry:
				case Usage_generic_desktop::Rz:
					//case Usage_generic_desktop::Vx:
					//case Usage_generic_desktop::Vy:
					//case Usage_generic_desktop::Vz:
					//case Usage_generic_desktop::Vbrx:
					//case Usage_generic_desktop::Vbry:
					//case Usage_generic_desktop::Vbrz:
					//case Usage_generic_desktop::Vno:
					{
						if(!axis_usage_set.emplace(cap.NotRange.Usage).second)
						{
							LOG_ERROR("Multiple axis with the same code "sv, cap.NotRange.Usage);
							return false;
						}

						if(cap.LogicalMin != 0)
						{
							break;
						}
						if(bit_size != 8 && bit_size != 16)
						{
							LOG_DEBUG("Bad size axis "sv, bit_size);
							return false;
						}
						uint8_t const num_bytes = static_cast<uint8_t>((bit_size / 8));


						out_axis_caps.emplace_back(
							Axis_cap{
								.usage_id = usage,
								.link_colection = cap.LinkCollection,
								.byte_size = num_bytes});
					}
					break;
				case Usage_generic_desktop::hat_switch:
					if(bit_size != 4)
					{
						LOG_DEBUG("Bad size hat "sv, bit_size);
						return false;
					}

					if(out_hat_caps.available)
					{
						LOG_DEBUG("Found Multiple Hat caps"sv);
						break;
					}

					out_hat_caps.available = true;
					out_hat_caps.link_colection = cap.LinkCollection;
					break;
				default:
					LOG_DEBUG("Unused val type "sv, cap.NotRange.Usage);
					break;
				}
			}
		}

		p_decoded_caps.input_report_size = report_lenght;
		p_decoded_caps.input_report_id = input_report_id;
		p_decoded_caps.m_button_array_cap = out_button_caps;
		p_decoded_caps.m_hat_cap = out_hat_caps;
		p_decoded_caps.m_axis_cap = std::move(out_axis_caps);

		return true;
	}

} //namespace input
