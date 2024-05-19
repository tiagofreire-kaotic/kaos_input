//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
/// 
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include "kaos_input_backend_api.h"
#include "backend_structures.hpp"


namespace kaos::input::backend
{
class manager;
class input_receiver;

class interface
{
	friend class backend_override;
public:
	kaos_input_backend_API interface();
	kaos_input_backend_API ~interface();

	kaos_input_backend_API bool startup(input_receiver& p_receiver);
	kaos_input_backend_API void shutdown();

private:
	manager* m_manager;
};

kaos_input_backend_API version_info version();

} //namespace kaos::input::backend
