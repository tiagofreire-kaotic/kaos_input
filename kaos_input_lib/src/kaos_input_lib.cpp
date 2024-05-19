//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		
//======== ======== ======== ======== ======== ======== ======== ========

#include "kaos_input_lib/kaos_input_lib.hpp"

#include <kaos_input/kaos_input.hpp>



namespace kaos::input
{
	namespace
	{
		void lib_add_device(void* const context, [[maybe_unused]] kaos::input::device_ingress const& p_data)
		{
			reinterpret_cast<input_manager*>(context)->add_device();//todo
		}

		void lib_remove_device(void* const context, [[maybe_unused]] kaos::input::device_id const p_data)
		{
			reinterpret_cast<input_manager*>(context)->remove_device();//todo
		}

	} //namespace


	input_manager::input_manager() = default;

	input_manager::~input_manager()
	{
		shutdown();
	}

	bool input_manager::startup()
	{
		input_receiver_1 receiver
		{
			.user_context	= this,
			.add_device		= lib_add_device,
			.remove_device	= lib_remove_device,
		};
		start_kaos_input(receiver, kaos::input::local_version);
		return true;
	}

	void input_manager::shutdown()
	{
		stop_kaos_input();
	}

} //namespace kaos::input
