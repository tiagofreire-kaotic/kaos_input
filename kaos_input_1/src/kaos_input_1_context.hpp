//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///		
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <kaos_input_backend/interface.hpp>
#include <kaos_input_backend/input_receiver.hpp>
#include <kaos_input_backend/event.hpp>
#include <kaos_input/kaos_input_structs.hpp>


#include <kaos_input_services/logger.hpp>

namespace kaos::input
{
	
	class Kaos_input_context: public interface, public input_receiver
	{
	public:
		Kaos_input_context();
		~Kaos_input_context();

		error startup(input_receiver_1 const& input_receiver);
		void shutdown();

	private:
		void add_device       (device_ingress_event_t const& p_event, device_handle_t const& p_device) override;
		void remove_device    (device_departure_event_t const& p_event, device_handle_t const& p_device) override;

		void mouse_move_event  (mouse_move_event_t   const& p_event, device_handle_t const& p_device) override;
		void mouse_wheel_event (mouse_button_event_t const& p_event, device_handle_t const& p_device) override;
		void mouse_button_event(mouse_wheel_event_t  const& p_event, device_handle_t const& p_device) override;

		void keyboard_event(keyboard_event_t const& p_event, device_handle_t const& p_device) override;


		input_receiver_1 m_user_callbacks;
		bool m_init_complete = false;

	};
} //namespace kaos::input
