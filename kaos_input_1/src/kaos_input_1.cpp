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


extern "C"
{

kaos_input_instance start_kaos_input(input_receiver_1 const& p_event_receiver)
{

	if(!kaos_input_services::start_services())
	{
		return nullptr;
	}

	kaos::input::Kaos_input_context* context = new kaos::input::Kaos_input_context; //clean this up

	bool const init_completed = context->startup(p_event_receiver);

	if(!init_completed)
	{
		delete context;
		return nullptr;
	}

	//TODO


	return reinterpret_cast<kaos_input_instance>(context);
}

void stop_kaos_input(kaos_input_instance const& p_context)
{
	if(p_context)
	{
		delete reinterpret_cast<kaos::input::Kaos_input_context*>(p_context);
	}

	kaos_input_services::stop_services();
}

} //extern "C"
