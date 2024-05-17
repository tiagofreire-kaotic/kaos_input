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


namespace kaos::input
{
std::basic_string<core::os_char> to_os_natural_convert(std::basic_string_view<char8_t> p_string);
std::basic_string<char8_t> from_os_natural_convert(std::basic_string_view<core::os_char> p_string);
} //namespace kaos::input
