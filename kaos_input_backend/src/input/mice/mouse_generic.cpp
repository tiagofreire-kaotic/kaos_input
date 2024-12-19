//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#include "mouse_generic.hpp"

#include <Windows.h>

#include <kaos_input_backend/event.hpp>
#include <kaos_input_backend/input_receiver.hpp>

#include <kaos_input_services/logger.hpp>

#include "../handle_maps.hpp"
#include "../../manager.hpp"

namespace kaos::input::backend
{

bool generic_mouse::init_device_characteristics(raw_device_handle_t p_handle, device_info_t p_deviceInfo)
{
	if(!device::init_device_characteristics(p_handle, p_deviceInfo))
	{
		return false;
	}

	const RID_DEVICE_INFO& tinfo = *to_native_handle(p_deviceInfo);

	LOG_INFO("Id                : "sv, tinfo.mouse.dwId					);
	LOG_INFO("NumberOfButtons   : "sv, tinfo.mouse.dwNumberOfButtons	);
	LOG_INFO("SampleRate        : "sv, tinfo.mouse.dwSampleRate			);
	LOG_INFO("HasHorizontalWheel: "sv, tinfo.mouse.fHasHorizontalWheel	);

	return true;
}

void generic_mouse::handle_input(raw_data_t p_handle, [[maybe_unused]] raw_header_t p_header)
{
	RAWINPUT data;
	UINT size = sizeof(data);
	UINT ret = GetRawInputData(to_native_handle(p_handle), RID_INPUT, &data, &size, sizeof(RAWINPUTHEADER));

	if(ret == UINT(-1))
	{
		return;
	}

	if(data.data.mouse.lLastX || data.data.mouse.lLastY)
	{
		mouse_move_event_t moveEvent { static_cast<int32_t>(data.data.mouse.lLastX), static_cast<int32_t>(data.data.mouse.lLastY) };
		get_manager().get_receiver().mouse_move_event(moveEvent, device_handle());
	}

	if(data.data.mouse.usButtonFlags)
	{

	}

	LOG_INFO ("Mouse "sv,				device_handle().handle());
	LOG_INFO ("usFlags           : "sv, data.data.mouse.usFlags				);
	LOG_INFO ("usButtonFlags     : "sv, data.data.mouse.usButtonFlags		);
	LOG_INFO ("usButtonData      : "sv, data.data.mouse.usButtonData		);
	LOG_INFO ("ulRawButtons      : "sv, data.data.mouse.ulRawButtons		);
	LOG_INFO ("lLastX            : "sv, data.data.mouse.lLastX				);
	LOG_INFO ("lLastY            : "sv, data.data.mouse.lLastY				);
	LOG_INFO ("ulExtraInformation: "sv, data.data.mouse.ulExtraInformation	);
}

} //namespace kaos::input::backend