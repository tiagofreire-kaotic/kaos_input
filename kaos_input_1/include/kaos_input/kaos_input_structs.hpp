//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <cstdint>
#include "kaos_input_api.h"


namespace kaos::input
{
	namespace _p
	{
		struct _kaos_input_instance_;
		struct _input_id_;
	}

	enum class device_category_t: uint8_t
	{
		mouse      = 0,
		keyboard   = 1,
		controller = 2,
	};

	enum class error: uint8_t
	{
		none                 = 0,
		already_running      = 1,
		invalid_arguments    = 2,
		failed_services      = 3,
		incompatible_version = 0xFE,
		internal_error       = 0xFF,
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


	using device_id_t = kaos::input::_p::_input_id_*;

	struct device_ingress_t
	{
		device_id_t    id;
		char8_t const* device_UUID;
		uint8_t        device_UUID_size;
		kaos::input::device_category_t device_category;
	};

	struct keyboard_event_t
	{
		device_id_t id;
		uint16_t key;
		bool pressed;
	};

	struct mouse_event_t
	{
	public:
		enum class sub_type_t : uint8_t
		{
			move   = 0,
			button = 1,
			wheel  = 2,
		};

		struct move_t
		{
			int32_t const m_x;
			int32_t const m_y;
		};

		struct button_t
		{
			uint16_t index;
			bool pressed;
		};

		struct wheel_t
		{
			uint16_t index;
			int16_t delta;
		};

	public:
		device_id_t id;
		sub_type_t sub_type;

		union 
		{
			move_t   move;
			button_t button;
			wheel_t  wheel;
		} data;
	};


	struct input_receiver_1
	{
		void* user_context = nullptr;
		void (* add_device    )(void*, device_ingress_t const&) = nullptr; //placeholder
		void (* remove_device )(void*, device_id_t)             = nullptr; //placeholder
		void (* keyboard_event)(void*, keyboard_event_t const&) = nullptr; //placeholder
		void (* mouse_event   )(void*, mouse_event_t const&)    = nullptr; //placeholder
	};


	namespace _p
	{
		consteval version_info_t local_version()
		{
			using discrete_t = decltype(::kaos::input::version_info_t{}.discrete);
			return version_info_t{.discrete = discrete_t{.reserved = 0, .revision = 0, .minor = 0, .major = 0}};
		}
	} // namespace _p

	constexpr version_info_t local_version = _p::local_version();
} // kaos::input
