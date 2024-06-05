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

#include <versionSpecific.h>

#include <kaos_input_services/logger.hpp>


static kaos::input::Kaos_input_context global_context;

extern "C"
{

kaos::input::error start_kaos_input(kaos::input::input_receiver_1 const& p_event_receiver, kaos::input::version_info_t const frontend_version)
{
	return global_context.startup(p_event_receiver, frontend_version);
}

void stop_kaos_input()
{
	global_context.shutdown();
}

kaos::input::version_info_t kaos_input_API_version()
{
	static_assert(kaos::input::local_version.discrete.reserved == 0);
	static_assert(kaos::input::local_version.discrete.revision == VERSION_REV);
	static_assert(kaos::input::local_version.discrete.minor    == VERSION_MINOR);
	static_assert(kaos::input::local_version.discrete.major    == VERSION_MAJOR);
	return kaos::input::local_version;
}

kaos::input::version_info_t kaos_input_backend_version()
{
	return kaos::input::version_info_t{.number = kaos::input::backend::version().number };
}

} //extern "C"
