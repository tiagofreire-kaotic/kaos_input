//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once
#include <string>
#include <string_view>
#include <CoreLib/string/core_os_string.hpp>


namespace kaos::input::backend
{
#if 0
std::basic_string<core::os_char> to_os_natural_convert(std::u8string_view p_string);
std::u8string from_os_natural_convert(std::basic_string_view<core::os_char> p_string);
#endif

std::u8string from_os_HID_convert(std::basic_string_view<core::os_char> p_string);

} //namespace kaos::input::backend
