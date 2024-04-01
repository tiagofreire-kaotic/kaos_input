//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Tiago Miguel Oliveira Freire
///		
///		Permission is hereby granted, free of charge, to any person obtaining a copy
///		of this software and associated documentation files (the "Software"), to deal
///		in the Software without restriction, including without limitation the rights
///		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
///		copies of the Software, and to permit persons to whom the Software is
///		furnished to do so, subject to the following conditions:
///		
///		The above copyright notice and this permission notice shall be included in all
///		copies or substantial portions of the Software.
///		
///		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
///		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
///		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
///		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
///		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
///		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
///		SOFTWARE.
//======== ======== ======== ======== ======== ======== ======== ========

#include "input_manager.hpp"

#include <kaos_input_backend/event.hpp>

#include <Logger/Logger.hpp>

bool input_manager::startup()
{
	return m_input_interface.startup(*this);
}

void input_manager::shutdown()
{
	m_input_interface.shutdown();
}

void input_manager::add_device()
{
}

void input_manager::remove_device()
{
}

void input_manager::mouse_move_event()
{
}

void input_manager::mouse_wheel_event()
{
}

void input_manager::mouse_key_event()
{
}

void input_manager::keyboard_event([[maybe_unused]] kaos::input::keyboard_event const& p_event, [[maybe_unused]] kaos::input::device_handle_t const& p_device)
{
	LOG_INFO("Key Event: "sv, p_event.key());

}
