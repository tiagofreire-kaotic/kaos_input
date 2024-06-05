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


	error Kaos_input_context::startup(input_receiver_1 const& input_receiver, version_info_t const frontend_version)
	{
		if(m_init_complete)
		{
			return error::already_running;
		}

		if(m_services_running)
		{
			backend::stop_services();
			m_services_running = false;
		}
		if(!backend::start_services())
		{
			return error::failed_services;
		}
		m_services_running = true;

		if(frontend_version.number > local_version.number)
		{
			LOG_ERROR(
				"API v"sv, local_version.discrete.major, '.', local_version.discrete.minor, '.', local_version.discrete.revision,
				" but application expects at least v"sv, frontend_version.discrete.major, '.', frontend_version.discrete.minor, '.', frontend_version.discrete.revision,
				". Please update the input software."sv);
			return error::incompatible_version;
		}

		if(frontend_version.discrete.major != local_version.discrete.major)
		{
			LOG_ERROR("API major version "sv, local_version.discrete.major, " but application expects major version "sv, frontend_version.discrete.major,
				". This is likely a linker error."sv);
			return error::incompatible_version;
		}

		{
			backend::version_info_t const backend_version = backend::version();
			if(backend_version.number != backend::local_version.number)
			{
				LOG_ERROR("Found backend v"sv, backend_version.discrete.major, '.', backend_version.discrete.minor, '.', backend_version.discrete.revision,
					" but expected v"sv, backend::local_version.discrete.major, '.', backend::local_version.discrete.minor, '.', backend::local_version.discrete.revision,
					". This is likely a software installation error."sv);
				return error::internal_error;
			}
		}

		if(
			   input_receiver.user_context   == nullptr
			|| input_receiver.add_device     == nullptr
			|| input_receiver.remove_device  == nullptr
			|| input_receiver.keyboard_event == nullptr
			)
		{
			return error::invalid_arguments;
		}
		m_user_callbacks = input_receiver;

		if(!interface::startup(*this))
		{
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
			m_init_complete = false;
		}
		if(m_services_running)
		{
			backend::stop_services();
			m_services_running = false;
		}
	}



	void Kaos_input_context::add_device(backend::device_ingress_event_t const& p_event, backend::device_handle_t const& p_device)
	{
		{
			using device_cat_underlying_t = std::underlying_type_t<device_category_t>;
			static_assert(std::is_same_v<device_cat_underlying_t, std::underlying_type_t<backend::DeviceType>>);
			static_assert(static_cast<device_cat_underlying_t>(device_category_t::mouse     ) == static_cast<device_cat_underlying_t>(backend::DeviceType::mouse));
			static_assert(static_cast<device_cat_underlying_t>(device_category_t::keyboard  ) == static_cast<device_cat_underlying_t>(backend::DeviceType::keyboard));
			static_assert(static_cast<device_cat_underlying_t>(device_category_t::controller) == static_cast<device_cat_underlying_t>(backend::DeviceType::controller));
		}

		device_ingress_t event_message
		{
			.id               = std::bit_cast<device_id_t>(p_device.id()),
			.device_UUID      = p_event.device_uuid.data(),
			.device_UUID_size = static_cast<uint8_t>(p_event.device_uuid.size()),
			.device_category  = static_cast<device_category_t>(p_event.device_type),
		};

		m_user_callbacks.add_device(m_user_callbacks.user_context, event_message);
	}


	void Kaos_input_context::remove_device([[maybe_unused]] backend::device_departure_event_t const& p_event, backend::device_handle_t const& p_device)
	{
		m_user_callbacks.remove_device(m_user_callbacks.user_context, std::bit_cast<device_id_t>(p_device.id()));
	}

	void Kaos_input_context::mouse_move_event([[maybe_unused]] backend::mouse_move_event_t const& p_event, [[maybe_unused]] backend::device_handle_t const& p_device)
	{
	}

	void Kaos_input_context::mouse_wheel_event([[maybe_unused]] backend::mouse_button_event_t const& p_event, [[maybe_unused]] backend::device_handle_t const& p_device)
	{
	}

	void Kaos_input_context::mouse_button_event([[maybe_unused]] backend::mouse_wheel_event_t  const& p_event, [[maybe_unused]] backend::device_handle_t const& p_device)
	{
	}

	void Kaos_input_context::keyboard_event(backend::keyboard_event_t const& p_event, backend::device_handle_t const& p_device)
	{
		keyboard_event_t event_message
		{
			.id      = std::bit_cast<device_id_t>(p_device.id()),
			.key     = p_event.key(),
			.pressed = p_event.pressed(),
		};
		m_user_callbacks.keyboard_event(m_user_callbacks.user_context, event_message);
	}

} //namespace kaos::input
