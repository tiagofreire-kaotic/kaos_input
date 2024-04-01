//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <cstdint>

namespace kaos::input
{
	enum class HID_page: uint16_t
	{
		undefined				= 0x0000,
		generic_desktop			= 0x0001,
		simulation_controls		= 0x0002,
		vr_controls				= 0x0003,
		sport_controls			= 0x0004,	//unused
		game_controls			= 0x0005,	//unused
		generic_device_controls	= 0x0006,	//unused
		keyboard				= 0x0007,
		LED						= 0x0008,	//unused
		button					= 0x0009,
		ordinal					= 0x000A,	//unused
		telephony				= 0x000B,	//unused
		consumer				= 0x000C,	//unused
		digitizer				= 0x000D,	//unused
		haptics					= 0x000E,
		PID						= 0x000F,	//unused
		unicode					= 0x0010,	//unused
		soc						= 0x0011,	//unused
		eye_head_tracker		= 0x0012,	//unused
		auxiliary_display		= 0x0014,	//unused
		sensor					= 0x0020,	//unused
		medical_instrument		= 0x0040,	//unused
		braille_display			= 0x0041,	//unused
		illumination			= 0x0059,	//unused
		monitor_0				= 0x0080,	//unused
		monitor_enum			= 0x0081,	//unused
		vesa					= 0x0082,	//unused
		monitor_3				= 0x0083,	//unused
		power_0					= 0x0084,	//unused
		battery					= 0x0085,	//unused
		power_2					= 0x0086,	//unused
		power_3					= 0x0087,	//unused
		bar_code_scanner		= 0x008C,	//unused
		scale					= 0x008D,	//unused
		magnetic_stripe_reader	= 0x008E,	//unused
		reserved_point_sale		= 0x008F,	//unused
		camera_control			= 0x0090,	//unused
		arcade					= 0x0091,	//unused
		gaming_device			= 0x0092,	//unused
		FIDO_aliance			= 0xF1D0,	//unused
		vendor_defined			= 0xFF00,	//unused
	};

	enum class Usage_generic_desktop: uint16_t
	{
		undefined						= 0x0000,
		pointer							= 0x0001,
		mouse							= 0x0002,
		joystick						= 0x0004,
		gamepad							= 0x0005,
		keyboard						= 0x0006,
		keypad							= 0x0007,
		multi_axis_controller			= 0x0008,
		tablet_pc_controls				= 0x0009,
		water_cooling_device			= 0x000A,
		computer_chassis_device			= 0x000B,
		wireless_radio_controls			= 0x000C,
		portable_device_control			= 0x000D,
		system_multi_axis_controller	= 0x000E,
		spacial_controller				= 0x000F,
		assistive_control				= 0x0010,
		device_dock						= 0x0011,
		dockable_device					= 0x0012,
		X								= 0x0030,
		Y								= 0x0031,
		Z								= 0x0032,
		Rx								= 0x0033,
		Ry								= 0x0034,
		Rz								= 0x0035,
		slider							= 0x0036,
		dial							= 0x0037,
		wheel							= 0x0038,
		hat_switch						= 0x0039,
		counted_buffer					= 0x003A,
		byte_count						= 0x003B,
		motion_wakeup					= 0x003C,
		start							= 0x003D,
		select							= 0x003E,
		Vx								= 0x0040,
		Vy								= 0x0041,
		Vz								= 0x0042,
		Vbrx							= 0x0043,
		Vbry							= 0x0044,
		Vbrz							= 0x0045,
		Vno								= 0x0046,
		feature_notification			= 0x0047,
		resolution_multiplier			= 0x0048,
		Qx								= 0x0049,
		Qy								= 0x004A,
		Qz								= 0x004B,
		Qw								= 0x004C,
		system_control					= 0x0080,
		system_power_down				= 0x0081,
		system_sleep					= 0x0082,
		system_wake_up					= 0x0083,
		system_context_menu				= 0x0084,
		system_main_menu				= 0x0085,
		system_app_menu					= 0x0086,
		system_menu_help				= 0x0087,
		system_menu_exit				= 0x0088,
		system_menu_select				= 0x0089,
		system_menu_right				= 0x008A,
		system_menu_left				= 0x008B,
		system_menu_up					= 0x008C,
		system_menu_down				= 0x008D,
		system_cold_restart				= 0x008E,
		system_warm_restart				= 0x008F,
		d_pad_up						= 0x0090,
		d_pad_down						= 0x0091,
		d_pad_right						= 0x0092,
		d_pad_left						= 0x0093,
		index_trigger					= 0x0094,
		palm_trigger					= 0x0095,
		thumbstick						= 0x0096,
		system_function_shift			= 0x0097,
		system_function_shift_lock		= 0x0098,
		system_function_shift_lock_indicator = 0x0099,
		system_dismiss_notification		= 0x009A,
		system_do_not_disturb			= 0x009B,
		system_dock						= 0x00A0,
		system_undock					= 0x00A1,
		system_setup					= 0x00A2,
		system_break					= 0x00A3,
		system_debugger_break			= 0x00A4,
		application_break				= 0x00A5,
		application_debugger_break		= 0x00A6,
		system_peaker_mute				= 0x00A7,
		system_hibernate				= 0x00A8,
		system_display_invert			= 0x00B0,
		system_display_internal			= 0x00B1,
		system_display_external			= 0x00B2,
		system_display_both				= 0x00B3,
		system_display_dual				= 0x00B4,
		system_display_toggle_int_ext	= 0x00B5,
		system_display_swap_prim_sec	= 0x00B6,
		system_display_toggle_LCD_autoscale = 0x00B7,
		sensor_zone						= 0x00C0,
		rpm								= 0x00C1,
		coolant_level					= 0x00C2,
		coolant_critical_level			= 0x00C3,
		coolant_pump					= 0x00C4,
		chassis_enclosure				= 0x00C5,
		wireless_radio_button			= 0x00C6,
		wireless_radio_LED				= 0x00C7,
		wireless_radio_slider_switch	= 0x00C8,
		system_display_rotation_lock_button = 0x00C9,
		system_display_rotation_lock_slider_switch	= 0x00CA,
		control_enabled					= 0x00CB,
		dockable_device_unique_ID		= 0x00D0,
		dockable_device_vendor_ID		= 0x00D1,
		dockable_device_primary_usage_page = 0x00D2,
		dockable_device_primary_usage_id = 0x00D3,
		dockable_device_docking_state	= 0x00D4,
		dockable_device_display_occlusion = 0x00D5,
		dockable_device_object_type		= 0x00D6,
	};

} //namespace kaos::input
