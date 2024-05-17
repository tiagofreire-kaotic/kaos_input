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
		none              = 0,
		already_running   = 1,
		invalid_arguments = 2,
		failed_services   = 3,
		internal_error    = 0xFF,
	};

	using device_id = kaos::input::_p::_input_id_*;

	struct device_ingress
	{
		device_id      id;
		char8_t const* device_UUID;
		uint8_t        device_UUID_size;
		kaos::input::device_category_t device_category;
	};


	struct input_receiver_1
	{
		void* user_context = nullptr;
		void (*add_device)   (void*, device_ingress const&) = nullptr; //placeholder
		void (*remove_device)(void*, device_id)             = nullptr; //placeholder

		//void (*mouse_move_event )(void*) = nullptr; //placeholder
		//void (*mouse_wheel_event)(void*) = nullptr; //placeholder
		//void (*mouse_key_event  )(void*) = nullptr; //placeholder
		//void (*keyboard_event)(void*,/*keyboard_event const&, device_handle_t const&*/) = nullptr; //placeholder
	};


}
