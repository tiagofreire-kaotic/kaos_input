//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#include "keyboard_generic.hpp"

#include <algorithm>

#include <Windows.h>

#include <kaos_input_services/logger.hpp>

#include <kaos_input_backend/event.hpp>
#include <kaos_input_backend/input_receiver.hpp>

#include "../handle_maps.hpp"
#include "../../manager.hpp"


namespace kaos::input::backend
{
	bool generic_keyboard::init_device_characteristics(raw_device_handle_t p_handle, device_info_t p_deviceInfo)
	{
		if(!device::init_device_characteristics(p_handle, p_deviceInfo))
		{
			return false;
		}

		const RID_DEVICE_INFO& tinfo = *to_native_handle(p_deviceInfo);

	//	LOG_INFO("Type                : "sv, tinfo.keyboard.dwType);
	//	LOG_INFO("SubType             : "sv, tinfo.keyboard.dwSubType);
	//	LOG_INFO("KeyboardMode        : "sv, tinfo.keyboard.dwKeyboardMode);
	//	LOG_INFO("NumberOfFunctionKeys: "sv, tinfo.keyboard.dwNumberOfFunctionKeys);
	//	LOG_INFO("NumberOfIndicators  : "sv, tinfo.keyboard.dwNumberOfIndicators);
	//	LOG_INFO("NumberOfKeysTotal   : "sv, tinfo.keyboard.dwNumberOfKeysTotal);

		m_number_of_keys = std::min(static_cast<uint16_t>(tinfo.keyboard.dwNumberOfKeysTotal), static_cast<uint16_t>(m_pressed_keys.size()));


		return true;
	}

	void generic_keyboard::handle_input(raw_data_t p_handle, [[maybe_unused]] raw_header_t p_header)
	{
		RAWINPUT data;
		UINT size = sizeof(data);
		UINT ret = GetRawInputData(to_native_handle(p_handle), RID_INPUT, &data, &size, sizeof(RAWINPUTHEADER));

		if(ret == UINT(-1))
		{
			return;
		}

		uint16_t const code = data.data.keyboard.MakeCode;

		if(code < m_number_of_keys)
		{
			bool& previous_state = m_pressed_keys[code];
			bool const new_state = (data.data.keyboard.Flags & RI_KEY_BREAK);

			if(new_state != previous_state)
			{
				previous_state = new_state;
				keyboard_event_t key_event{code};
				get_manager().get_receiver().keyboard_event(key_event, device_handle());
			}
		}
		else
		{
			LOG_WARNING("Key "sv, code, " exceeds max of "sv, m_number_of_keys);
		}

	//	LOG_INFO("Keyboard "sv, deviceHandle().handle);
	//	LOG_INFO("MakeCode        : "sv, data.data.keyboard.MakeCode);
	//	LOG_INFO("Flags           : "sv, data.data.keyboard.Flags);
	//	LOG_INFO("Reserved        : "sv, data.data.keyboard.Reserved);
	//	LOG_INFO("VKey            : "sv, data.data.keyboard.VKey);
	//	LOG_INFO("Message         : "sv, data.data.keyboard.Message);
	//	LOG_INFO("ExtraInformation: "sv, data.data.keyboard.ExtraInformation);

	}

} //namespace kaos::input::backend
