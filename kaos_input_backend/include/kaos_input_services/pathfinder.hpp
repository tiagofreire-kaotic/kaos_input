//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include "kaos_input_backend/kaos_input_backend_api.h"

#include <filesystem>
#include <string_view>

namespace kaos::input::backend
{
	kaos_input_backend_API const std::filesystem::path& path_find(std::u8string_view p_category);
} //namespace kaos::input::backend
