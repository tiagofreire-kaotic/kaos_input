//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///		
//======== ======== ======== ======== ======== ======== ======== ========

#include "kaos_input_1_context.hpp"
#include <kaos_input_backend/device_handle.hpp>



namespace kaos::input
{
	Kaos_input_context::Kaos_input_context () = default;
	Kaos_input_context::~Kaos_input_context() = default;

	bool Kaos_input_context::startup(input_receiver_1 const& input_receiver)
	{
		if(
			   input_receiver.user_context  == nullptr
			|| input_receiver.add_device    == nullptr
			|| input_receiver.remove_device == nullptr
			)
		{
			LOG_INFO("Invalid arguments"sv);
			return false;
		}
		m_user_callbacks = input_receiver;
		return interface::startup(*this);
	}

	void Kaos_input_context::add_device()
	{
		LOG_INFO();

		//TODO
		//kaos_input_ingress ingress_event;

		//m_user_callbacks.add_device(m_user_callbacks.user_context, );

	}

	void Kaos_input_context::remove_device()
	{
	}

	void Kaos_input_context::mouse_move_event ()
	{
	}
	void Kaos_input_context::mouse_wheel_event()
	{
	}
	void Kaos_input_context::mouse_key_event  ()
	{
	}

	void Kaos_input_context::keyboard_event(keyboard_event_t const& p_event, device_handle_t const& p_device)
	{
		LOG_INFO("Key Event: key="sv, p_event.key(), " id="sv, p_device.id());
	}

} //namespace kaos::input
