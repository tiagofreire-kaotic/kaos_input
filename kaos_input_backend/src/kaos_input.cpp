//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#include <kaos_input_backend/interface.hpp>

#include "manager.hpp"

namespace kaos::input
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

}	// kaos::input
