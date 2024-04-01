//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include "handle_types.hpp"
#include "handle_maps_p.hpp"

#include <Windows.h>

#define HANDLE_MAPPING(Generic, Native) \
	template<> struct _to_native_handle_help_s<Generic> { using peer_type = Native; }; \
	template<> struct _to_generic_handle_help_s<Native> { using peer_type = Generic; };


namespace kaos::input
{
	HANDLE_MAPPING(raw_device_handle_t, HANDLE);
	HANDLE_MAPPING(device_info_t, PRID_DEVICE_INFO);
	HANDLE_MAPPING(raw_data_t, HRAWINPUT);
	HANDLE_MAPPING(raw_header_t, PRAWINPUTHEADER);
}

#undef HANDLE_MAPPING
