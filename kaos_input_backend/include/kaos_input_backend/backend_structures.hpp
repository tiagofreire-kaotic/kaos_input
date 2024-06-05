//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <cstdint>
#include <bit>

namespace kaos::input::backend
{
	enum class DeviceType: uint8_t
	{
		mouse      = 0,
		keyboard   = 1,
		controller = 2,
	};

	union version_info_t
	{
		struct
		{
			uint16_t reserved;
			uint16_t revision;
			uint16_t minor;
			uint16_t major;
		} discrete;
		uint64_t number;
	};


	namespace _p
	{
		consteval version_info_t local_version()
		{
			using discrete_t = decltype(::kaos::input::backend::version_info_t{}.discrete);
			return version_info_t{.discrete = discrete_t{.reserved = 0, .revision = 0, .minor = 0, .major = 0}};
		}
	} // namespace _p

	constexpr version_info_t local_version = _p::local_version();
} //namespace kaos::input

