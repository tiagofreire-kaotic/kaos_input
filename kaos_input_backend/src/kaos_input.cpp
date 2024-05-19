//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#include <kaos_input_backend/interface.hpp>

#include "manager.hpp"
#include <versionSpecific.h>

namespace kaos::input::backend
{

	interface::interface() : m_manager{ new manager }
	{

	}

	interface::~interface()
	{
		delete m_manager;
	}

	bool interface::startup(input_receiver& p_receiver)
	{
		return m_manager->startup(p_receiver);
	}

	void interface::shutdown()
	{
		m_manager->shutdown();
	}


	version_info version()
	{
		static_assert(local_version.discrete.reserved == 0);
		static_assert(local_version.discrete.revision == VERSION_REV);
		static_assert(local_version.discrete.minor    == VERSION_MINOR);
		static_assert(local_version.discrete.major    == VERSION_MAJOR);
		return local_version;
	}

} // namespace kaos::input::backend
