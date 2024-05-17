//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///		
//======== ======== ======== ======== ======== ======== ======== ========

#include <kaos_input/kaos_input.hpp>
#include <kaos_input_services/services.hpp>
#include "kaos_input_1_context.hpp"


#include <kaos_input_services/logger.hpp>


static kaos::input::Kaos_input_context global_context;

extern "C"
{

kaos::input::error start_kaos_input(kaos::input::input_receiver_1 const& p_event_receiver)
{
	return global_context.startup(p_event_receiver);
}	

void stop_kaos_input()
{
	global_context.shutdown();
}

} //extern "C"
