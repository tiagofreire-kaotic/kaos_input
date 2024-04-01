//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <cstdint>
#include "input_manager.hpp"

class window_app
{
public:
	intptr_t dispatch_message(void* p_handle, uint32_t p_message, uintptr_t p_Wparam, intptr_t p_Lparam);

	int run(void* p_instance);

	void notify_quit();

private:
	bool m_should_quit = false;

	input_manager m_input_manager;

	//GraphicHandler m_graphic_API;

private: //temp
	//unique_api_ptr<WindowContext> m_window_context;
};

