//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#include "HID_cap_print.hpp"

#include <kaos_input_services/logger.hpp>

#include <CoreLib/Core_Alloca.hpp>
#include <CoreLib/Core_extra_compiler.hpp>

namespace kaos::input::backend
{

	void print_linked_caps(PHIDP_PREPARSED_DATA p_data, uint16_t p_count);
	void print_button_caps(PHIDP_PREPARSED_DATA p_data, uint16_t p_count, HIDP_REPORT_TYPE p_type);
	void print_value_caps(PHIDP_PREPARSED_DATA p_data, uint16_t p_count, HIDP_REPORT_TYPE p_type);

	void print_HID_caps(PHIDP_PREPARSED_DATA const p_data)
	{
		HIDP_CAPS capabilities;
		if(HidP_GetCaps(p_data, &capabilities) != HIDP_STATUS_SUCCESS)
		{
			LOG_INFO("Failed to get caps"sv);
			return;
		}

		LOG_INFO("Usage                    : "sv, core::toPrint_hex_fix{capabilities.Usage});
		LOG_INFO("UsagePage                : "sv, core::toPrint_hex_fix{capabilities.UsagePage});
		LOG_INFO("InputReportByteLength    : "sv, capabilities.InputReportByteLength);
		LOG_INFO("OutputReportByteLength   : "sv, capabilities.OutputReportByteLength);
		LOG_INFO("FeatureReportByteLength  : "sv, capabilities.FeatureReportByteLength);
		LOG_INFO("NumberLinkCollectionNodes: "sv, capabilities.NumberLinkCollectionNodes);
		LOG_INFO("NumberInputButtonCaps    : "sv, capabilities.NumberInputButtonCaps);
		LOG_INFO("NumberInputValueCaps     : "sv, capabilities.NumberInputValueCaps);
		LOG_INFO("NumberInputDataIndices   : "sv, capabilities.NumberInputDataIndices);
		LOG_INFO("NumberOutputButtonCaps   : "sv, capabilities.NumberOutputButtonCaps);
		LOG_INFO("NumberOutputValueCaps    : "sv, capabilities.NumberOutputValueCaps);
		LOG_INFO("NumberOutputDataIndices  : "sv, capabilities.NumberOutputDataIndices);
		LOG_INFO("NumberFeatureButtonCaps  : "sv, capabilities.NumberFeatureButtonCaps);
		LOG_INFO("NumberFeatureValueCaps   : "sv, capabilities.NumberFeatureValueCaps);
		LOG_INFO("NumberFeatureDataIndices : "sv, capabilities.NumberFeatureDataIndices);

		print_linked_caps(p_data, capabilities.NumberLinkCollectionNodes);

		if(capabilities.NumberInputButtonCaps)
		{
			print_button_caps(p_data, capabilities.NumberInputButtonCaps, HidP_Input);
		}
		if(capabilities.NumberOutputButtonCaps)
		{
			print_button_caps(p_data, capabilities.NumberOutputButtonCaps, HidP_Output);
		}
		if(capabilities.NumberFeatureButtonCaps)
		{
			print_button_caps(p_data, capabilities.NumberFeatureButtonCaps, HidP_Feature);
		}

		if(capabilities.NumberInputValueCaps)
		{
			print_value_caps(p_data, capabilities.NumberInputValueCaps, HidP_Input);
		}
		if(capabilities.NumberOutputValueCaps)
		{
			print_value_caps(p_data, capabilities.NumberOutputValueCaps, HidP_Output);
		}
		if(capabilities.NumberFeatureValueCaps)
		{
			print_value_caps(p_data, capabilities.NumberFeatureValueCaps, HidP_Feature);
		}
	}


	NO_INLINE void print_linked_caps(PHIDP_PREPARSED_DATA const p_data, uint16_t const p_count)
	{
		ULONG count = p_count;
		PHIDP_LINK_COLLECTION_NODE linked_nodes = reinterpret_cast<PHIDP_LINK_COLLECTION_NODE>(core_alloca(sizeof(HIDP_LINK_COLLECTION_NODE) * p_count));

		if(
			HidP_GetLinkCollectionNodes(linked_nodes, &count, p_data)
			== HIDP_STATUS_SUCCESS)
		{
			LOG_INFO("Linked Node"sv);
			for(ULONG i = 0; i < count; ++i)
			{
				HIDP_LINK_COLLECTION_NODE& caps = linked_nodes[i];
				LOG_INFO(i, ':');
				LOG_INFO("LinkUsage       : "sv, core::toPrint_hex_fix{caps.LinkUsage});
				LOG_INFO("LinkUsagePage   : "sv, core::toPrint_hex_fix{caps.LinkUsagePage});
				LOG_INFO("Parent          : "sv, caps.Parent);
				LOG_INFO("NumberOfChildren: "sv, caps.NumberOfChildren);
				LOG_INFO("NextSibling     : "sv, caps.NextSibling);
				LOG_INFO("FirstChild      : "sv, caps.FirstChild);
				LOG_INFO("CollectionType  : "sv, static_cast<uint8_t>(caps.CollectionType));
				LOG_INFO("IsAlias         : "sv, static_cast<bool>(caps.IsAlias));
				LOG_INFO("UserContext     : "sv, caps.UserContext);
			}
			return;
		}
		LOG_INFO("Failed to retrieve Link information"sv);
	}

	NO_INLINE void print_button_caps(PHIDP_PREPARSED_DATA const p_data, uint16_t const p_count, HIDP_REPORT_TYPE const p_type)
	{
		uint16_t count = p_count;
		PHIDP_BUTTON_CAPS button_capabilities = reinterpret_cast<PHIDP_BUTTON_CAPS>(core_alloca(sizeof(HIDP_BUTTON_CAPS) * p_count));
		if(
			HidP_GetButtonCaps(p_type, button_capabilities, &count, p_data)
			== HIDP_STATUS_SUCCESS
			)
		{
			LOG_INFO("==== Bt type: "sv, static_cast<uint8_t>(p_type));
			if(count != p_count)
			{
				LOG_INFO("Retrieved count differs "sv, count, " expectd "sv, p_count);
			}

			for(uint16_t i = 0; i < count; ++i)
			{
				HIDP_BUTTON_CAPS& caps = button_capabilities[i];
				LOG_INFO(i, ':');

				LOG_INFO("UsagePage        : "sv, core::toPrint_hex_fix{caps.UsagePage});
				LOG_INFO("ReportID         : "sv, caps.ReportID);
				LOG_INFO("IsAlias          : "sv, caps.IsAlias);
				LOG_INFO("BitField         : "sv, caps.BitField);
				LOG_INFO("LinkCollection   : "sv, caps.LinkCollection);
				LOG_INFO("LinkUsage        : "sv, core::toPrint_hex_fix{caps.LinkUsage});
				LOG_INFO("LinkUsagePage    : "sv, core::toPrint_hex_fix{caps.LinkUsagePage});
				LOG_INFO("IsRange          : "sv, caps.IsRange);
				LOG_INFO("IsStringRange    : "sv, caps.IsStringRange);
				LOG_INFO("IsDesignatorRange: "sv, caps.IsDesignatorRange);
				LOG_INFO("IsAbsolute       : "sv, caps.IsAbsolute);
				LOG_INFO("ReportCount      : "sv, caps.ReportCount);
				LOG_INFO("Reserved2        : "sv, caps.Reserved2);

				if(caps.IsRange)
				{
					LOG_INFO("Range.UsageMin     : "sv, core::toPrint_hex_fix{caps.Range.UsageMin});
					LOG_INFO("Range.UsageMax     : "sv, core::toPrint_hex_fix{caps.Range.UsageMax});
					LOG_INFO("Range.StringMin    : "sv, caps.Range.StringMin);
					LOG_INFO("Range.StringMax    : "sv, caps.Range.StringMax);
					LOG_INFO("Range.DesignatorMin: "sv, caps.Range.DesignatorMin);
					LOG_INFO("Range.DesignatorMax: "sv, caps.Range.DesignatorMax);
					LOG_INFO("Range.DataIndexMin : "sv, caps.Range.DataIndexMin);
					LOG_INFO("Range.DataIndexMax : "sv, caps.Range.DataIndexMax);
				}
				else
				{
					LOG_INFO("NotRange.Usage          : "sv, core::toPrint_hex_fix{caps.NotRange.Usage});
					LOG_INFO("NotRange.StringIndex    : "sv, caps.NotRange.StringIndex);
					LOG_INFO("NotRange.DesignatorIndex: "sv, caps.NotRange.DesignatorIndex);
					LOG_INFO("NotRange.DataIndex      : "sv, caps.NotRange.DataIndex);
				}
			}
			return;
		}
		LOG_INFO("Failed to get button information class "sv, static_cast<uint8_t>(p_type));
	}

	NO_INLINE void print_value_caps(PHIDP_PREPARSED_DATA const p_data, uint16_t const p_count, HIDP_REPORT_TYPE const p_type)
	{
		PHIDP_VALUE_CAPS value_capabilities = reinterpret_cast<PHIDP_VALUE_CAPS>(core_alloca(sizeof(HIDP_VALUE_CAPS) * p_count));
		uint16_t count = p_count;
		if(
			HidP_GetValueCaps(p_type, value_capabilities, &count, p_data)
			== HIDP_STATUS_SUCCESS
			)
		{
			LOG_INFO("==== Val type: "sv, static_cast<uint8_t>(p_type));
			if(count != p_count)
			{
				LOG_INFO("Retrieved count differs "sv, count, " expectd "sv, p_count);
			}

			for(uint16_t i = 0; i < count; ++i)
			{
				HIDP_VALUE_CAPS& caps = value_capabilities[i];
				LOG_INFO(i, ':');
				LOG_INFO("UsagePage        : "sv, core::toPrint_hex_fix{caps.UsagePage});
				LOG_INFO("ReportID         : "sv, caps.ReportID);
				LOG_INFO("IsAlias          : "sv, caps.IsAlias);
				LOG_INFO("BitField         : "sv, caps.BitField);
				LOG_INFO("LinkCollection   : "sv, caps.LinkCollection);
				LOG_INFO("LinkUsage        : "sv, core::toPrint_hex_fix{caps.LinkUsage});
				LOG_INFO("LinkUsagePage    : "sv, core::toPrint_hex_fix{caps.LinkUsagePage});
				LOG_INFO("IsRange          : "sv, caps.IsRange);
				LOG_INFO("IsStringRange    : "sv, caps.IsStringRange);
				LOG_INFO("IsDesignatorRange: "sv, caps.IsDesignatorRange);
				LOG_INFO("IsAbsolute       : "sv, caps.IsAbsolute);
				LOG_INFO("HasNull          : "sv, caps.HasNull);
				LOG_INFO("BitSize          : "sv, caps.BitSize);
				LOG_INFO("ReportCount      : "sv, caps.ReportCount);
				LOG_INFO("UnitsExp         : "sv, caps.UnitsExp);
				LOG_INFO("Units            : "sv, caps.Units);
				LOG_INFO("LogicalMin       : "sv, caps.LogicalMin);
				LOG_INFO("LogicalMax       : "sv, caps.LogicalMax);
				LOG_INFO("PhysicalMin      : "sv, caps.PhysicalMin);
				LOG_INFO("PhysicalMax      : "sv, caps.PhysicalMax);

				if(caps.IsRange)
				{
					LOG_INFO("Range.UsageMin     : "sv, core::toPrint_hex_fix{caps.Range.UsageMin});
					LOG_INFO("Range.UsageMax     : "sv, core::toPrint_hex_fix{caps.Range.UsageMax});
					LOG_INFO("Range.StringMin    : "sv, caps.Range.StringMin);
					LOG_INFO("Range.StringMax    : "sv, caps.Range.StringMax);
					LOG_INFO("Range.DesignatorMin: "sv, caps.Range.DesignatorMin);
					LOG_INFO("Range.DesignatorMax: "sv, caps.Range.DesignatorMax);
					LOG_INFO("Range.DataIndexMin : "sv, caps.Range.DataIndexMin);
					LOG_INFO("Range.DataIndexMax : "sv, caps.Range.DataIndexMax);
				}
				else
				{
					LOG_INFO("NotRange.Usage          : "sv, core::toPrint_hex_fix{caps.NotRange.Usage});
					LOG_INFO("NotRange.StringIndex    : "sv, caps.NotRange.StringIndex);
					LOG_INFO("NotRange.DesignatorIndex: "sv, caps.NotRange.DesignatorIndex);
					LOG_INFO("NotRange.DataIndex      : "sv, caps.NotRange.DataIndex);
				}
			}
			return;
		}
		LOG_INFO("Failed to get value information class "sv, static_cast<uint8_t>(p_type));
	}


} //namespace kaos::input::backend
