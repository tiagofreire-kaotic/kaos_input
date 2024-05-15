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




extern "C"
{

enum class device_category: uint8_t
{
	keyboard	= 0x00,
	mouse		= 0x01,
	control		= 0x02,
};


struct kaos_input_id //tbd
{
	
};

struct kaos_input_ingress
{
	kaos_input_id	id;
	char8_t*		device_name;
	uint8_t			device_name_size;
	device_category	device_category;
};


struct input_receiver_1
{
	void* user_context = nullptr;
	void (*add_device)   (void*, kaos_input_ingress const&) = nullptr; //placeholder
	void (*remove_device)(void*, kaos_input_id const&)      = nullptr; //placeholder

	//void (*mouse_move_event )(void*) = nullptr; //placeholder
	//void (*mouse_wheel_event)(void*) = nullptr; //placeholder
	//void (*mouse_key_event  )(void*) = nullptr; //placeholder
	//void (*keyboard_event)(void*,/*keyboard_event const&, device_handle_t const&*/) = nullptr; //placeholder
};


struct _kaos_input_instance_;
using kaos_input_instance = _kaos_input_instance_*;

} //extern "C"
