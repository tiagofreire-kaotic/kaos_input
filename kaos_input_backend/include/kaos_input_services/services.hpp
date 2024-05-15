//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <kaos_input_backend/kaos_input_backend_api.h>

namespace kaos_input_services
{
	kaos_input_backend_API bool start_services();
	kaos_input_backend_API void stop_services();
};
