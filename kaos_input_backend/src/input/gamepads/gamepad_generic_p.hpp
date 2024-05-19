//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include "gamepad_generic.hpp"
#include <span>

namespace kaos::input::backend
{
	class Button_data_collector: public DataPoint_Collector
	{
	public:
		inline Button_data_collector(std::span<Button> p_buttons, uint16_t const p_linked_collection)
			: DataPoint_Collector()
			, m_buttons(p_buttons)
			, m_linked_collection(p_linked_collection)
		{}

		void collect(void* p_data, uintptr_t p_data_size, void* p_preparsed_data) override;

	private:
		std::span<Button> const m_buttons;
		uint16_t const m_linked_collection;
	};

	class HatSwitch_data_collector: public DataPoint_Collector
	{
	public:
		inline HatSwitch_data_collector(HatSwitch& p_hatSwitch, uint16_t const p_linked_collection)
			: DataPoint_Collector()
			, m_hatSwitch(p_hatSwitch)
			, m_linked_collection(p_linked_collection)
		{}
		void collect(void* p_data, uintptr_t p_data_size, void* p_preparsed_data) override;

	private:
		HatSwitch& m_hatSwitch;
		uint16_t const m_linked_collection;
	};

	class Axis_data_collector: public DataPoint_Collector
	{
	public:
		inline Axis_data_collector(Axis& p_axis, uint16_t const p_linked_collection, Usage_generic_desktop const p_usage)
			: DataPoint_Collector()
			, m_axis(p_axis)
			, m_linked_collection(p_linked_collection)
			, m_usage(static_cast<uint16_t>(p_usage))
		{}

	protected:
		Axis& m_axis;
		uint16_t const m_linked_collection;
		uint16_t const m_usage;
	};

	class u8Axis_data_collector: public Axis_data_collector
	{
	public:
		inline u8Axis_data_collector(Axis& p_axis, uint16_t const p_linked_collection, Usage_generic_desktop const p_usage)
			: Axis_data_collector(p_axis, p_linked_collection, p_usage)
		{}
		void collect(void* p_data, uintptr_t p_data_size, void* p_preparsed_data) override;
	};

	class u16Axis_data_collector: public Axis_data_collector
	{
	public:
		u16Axis_data_collector(Axis& p_axis, uint16_t const p_linked_collection, Usage_generic_desktop const p_usage)
			: Axis_data_collector(p_axis, p_linked_collection, p_usage)
		{}
		void collect(void* p_data, uintptr_t p_data_size, void* p_preparsed_data) override;

	};

} //namespace kaos::input::backend
