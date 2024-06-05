//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========


#include "kaos_input_api.h"
#include "kaos_input/kaos_input_structs.hpp"

#pragma once

extern "C"
{
//[[nodiscard]]
kaos_input_API kaos::input::error start_kaos_input(kaos::input::input_receiver_1 const&, kaos::input::version_info_t const);
kaos_input_API void stop_kaos_input();

kaos_input_API kaos::input::version_info_t kaos_input_API_version();
kaos_input_API kaos::input::version_info_t kaos_input_backend_version();

} //extern "C"
