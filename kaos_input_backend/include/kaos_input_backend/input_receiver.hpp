//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
/// 
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include "kaos_input_backend_api.h"


namespace kaos::input
{
	class keyboard_event_t;
	class device_handle_t;

	class input_receiver
	{
	public:
		kaos_input_backend_API virtual ~input_receiver();

		virtual void add_device() = 0;
		virtual void remove_device() = 0;

		virtual void mouse_move_event () = 0;
		virtual void mouse_wheel_event() = 0;
		virtual void mouse_key_event  () = 0;

		virtual void keyboard_event(keyboard_event_t const& p_event, device_handle_t const& p_device) = 0;
	};

} //namespace kaos::input
