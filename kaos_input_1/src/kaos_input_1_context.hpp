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

struct input_receiver_1;

namespace kaos::input
{
	
	class Kaos_input_context: public interface, public input_receiver
	{
	public:
		Kaos_input_context();
		~Kaos_input_context();

		bool startup(input_receiver_1 const& input_receiver);

	private:
		void add_device() override;
		void remove_device() override;

		void mouse_move_event () override;
		void mouse_wheel_event() override;
		void mouse_key_event  () override;

		void keyboard_event(keyboard_event_t const& p_event, device_handle_t const& p_device) override;


		input_receiver_1 m_user_callbacks;

	};
} //namespace kaos::input
