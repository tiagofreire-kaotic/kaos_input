//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

namespace kaos::input
{
	class input_manager
	{
	public:
		input_manager();
		~input_manager();

		bool startup();
		void shutdown();

	public:
		virtual void add_device       () = 0;
		virtual void remove_device    () = 0;
		virtual void mouse_move_event () = 0;
		virtual void mouse_wheel_event() = 0;
		virtual void mouse_key_event  () = 0;
		virtual void keyboard_event   (/*kaos::input::keyboard_event const& p_event, kaos::input::device_handle_t const& p_device*/) = 0;

	private:

	};

} //kaos::input
