//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#define MAKE_REPLACE_HANDLE(X) struct X{ void* handle; inline constexpr bool operator < (const X& p_2) const { return handle < p_2.handle; } };

namespace kaos::input::backend
{
	MAKE_REPLACE_HANDLE(raw_device_handle_t);
	MAKE_REPLACE_HANDLE(device_info_t);
	MAKE_REPLACE_HANDLE(raw_data_t);
	MAKE_REPLACE_HANDLE(raw_header_t);

} //namespace kaos::input::backend

#undef MAKE_REPLACE_HANDLE
