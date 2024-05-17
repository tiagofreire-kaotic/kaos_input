//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///		
//======== ======== ======== ======== ======== ======== ======== ========

#include "kaos_input_1_context.hpp"
#include <kaos_input_backend/device_handle.hpp>
#include <kaos_input_services/services.hpp>

#include <bit>


namespace kaos::input
{
	Kaos_input_context::Kaos_input_context () = default;
	Kaos_input_context::~Kaos_input_context() = default;

	error Kaos_input_context::startup(input_receiver_1 const& input_receiver)
	{
		if(m_init_complete)
		{
			return error::already_running;
		}

		if(
			   input_receiver.user_context  == nullptr
			|| input_receiver.add_device    == nullptr
			|| input_receiver.remove_device == nullptr
			)
		{
			LOG_INFO("Invalid arguments"sv);
			return error::invalid_arguments;
		}
		m_user_callbacks = input_receiver;

		if(!kaos_input_services::start_services())
		{
			return error::failed_services;
		}

		if(!interface::startup(*this))
		{
			kaos_input_services::stop_services();

			return error::internal_error;
		}
		m_init_complete = true;
		return error::none;
	}

	void Kaos_input_context::shutdown()
	{
		if(m_init_complete)
		{
			interface::shutdown();
			kaos_input_services::stop_services();
			m_init_complete = false;
		}
	}

	void Kaos_input_context::add_device(device_ingress_event_t const& p_event, device_handle_t const& p_device)
	{
		device_ingress event_message
		{
			.id               = std::bit_cast<device_id>(p_device.id()),
			.device_UUID      = p_event.device_uuid.data(),
			.device_UUID_size = static_cast<uint8_t>(p_event.device_uuid.size()),
			.device_category  = static_cast<device_category_t>(p_event.device_type),
		};

		m_user_callbacks.add_device(m_user_callbacks.user_context, event_message);
	}


	void Kaos_input_context::remove_device([[maybe_unused]] device_departure_event_t const& p_event, [[maybe_unused]] device_handle_t const& p_device)
	{
	}

	void Kaos_input_context::mouse_move_event([[maybe_unused]] mouse_move_event_t const& p_event, [[maybe_unused]] device_handle_t const& p_device)
	{
	}

	void Kaos_input_context::mouse_wheel_event([[maybe_unused]] mouse_button_event_t const& p_event, [[maybe_unused]] device_handle_t const& p_device)
	{
	}

	void Kaos_input_context::mouse_button_event([[maybe_unused]] mouse_wheel_event_t  const& p_event, [[maybe_unused]] device_handle_t const& p_device)
	{
	}

	void Kaos_input_context::keyboard_event([[maybe_unused]] keyboard_event_t const& p_event, [[maybe_unused]] device_handle_t const& p_device)
	{
		LOG_INFO("Key Event: key="sv, p_event.key(), " id="sv, p_device.id());
	}

} //namespace kaos::input
