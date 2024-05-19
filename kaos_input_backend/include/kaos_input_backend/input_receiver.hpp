//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
/// 
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include "kaos_input_backend_api.h"


namespace kaos::input::backend
{
	class device_handle_t;
	class device_ingress_event_t;
	class device_departure_event_t;

	class keyboard_event_t;
	class mouse_move_event_t;
	class mouse_button_event_t;
	class mouse_wheel_event_t;



	class input_receiver
	{
	public:
		kaos_input_backend_API virtual ~input_receiver();

		virtual void add_device   (device_ingress_event_t   const& p_event, device_handle_t const& p_device) = 0;
		virtual void remove_device(device_departure_event_t const& p_event, device_handle_t const& p_device) = 0;

		virtual void mouse_move_event  (mouse_move_event_t   const& p_event, device_handle_t const& p_device) = 0;
		virtual void mouse_wheel_event (mouse_button_event_t const& p_event, device_handle_t const& p_device) = 0;
		virtual void mouse_button_event(mouse_wheel_event_t  const& p_event, device_handle_t const& p_device) = 0;

		virtual void keyboard_event(keyboard_event_t const& p_event, device_handle_t const& p_device) = 0;
	};

} //namespace kaos::input::backend
