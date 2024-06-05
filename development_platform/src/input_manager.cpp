//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#include "input_manager.hpp"

//#include <kaos_input_backend/event.hpp>

#include <Logger/Logger.hpp>

bool input_manager::startup()
{
	return kaos::input::input_manager::startup();
}

void input_manager::shutdown()
{
	kaos::input::input_manager::shutdown();
}

void input_manager::add_device()
{
	LOG_INFO("add_device"sv);
}

void input_manager::remove_device()
{
	LOG_INFO("remove_device"sv);
}

void input_manager::mouse_move_event()
{
	LOG_INFO("mouse_move_event"sv);
}

void input_manager::mouse_wheel_event()
{
	LOG_INFO("mouse_wheel_event"sv);
}

void input_manager::mouse_key_event()
{
	LOG_INFO("mouse_key_event"sv);
}

void input_manager::keyboard_event(/*[[maybe_unused]] kaos::input::keyboard_event const& p_event, [[maybe_unused]] kaos::input::device_handle_t const& p_device*/)
{
	LOG_INFO("Key Event: "sv /*, p_event.key()*/);
}
