//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#include "device.hpp"

#include <Windows.h>

#include <CoreLib/Core_Alloca.hpp>
#include <CoreLib/Core_extra_compiler.hpp>

#include <kaos_input_services/logger.hpp>

#include "../misc/os_string_string_conversion.hpp"

#include "handle_maps.hpp"

namespace kaos::input
{
	device::~device() = default;

	NO_INLINE bool device::init_device_characteristics(raw_device_handle_t p_handle, [[maybe_unused]] device_info_t p_deviceInfo)
	{
		m_handle = p_handle;
		UINT tsize = 0;
		const HANDLE thandle = to_native_handle(p_handle);

		UINT ret = GetRawInputDeviceInfoW(thandle, RIDI_DEVICENAME, nullptr, &tsize);

		if(ret == UINT(-1) || tsize > 1024 || tsize == 0)
		{
			return false;
		}
		const UINT expected_size = tsize;
		wchar_t* name = reinterpret_cast<wchar_t*>(core_alloca(tsize * sizeof(wchar_t)));

		ret = GetRawInputDeviceInfoW(thandle, RIDI_DEVICENAME, name, &tsize);

		if(ret != expected_size)
		{
			return false;
		}

		std::wstring_view uuid{name, expected_size - 1};
		LOG_INFO(uuid);

		m_uuid = from_os_natural_convert(uuid);

		return true;
	}
} //namespace input
