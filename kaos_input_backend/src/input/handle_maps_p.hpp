//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

namespace kaos::input
{
	namespace
	{
		template<typename T>
		struct _to_native_handle_help_s;

		template<typename T>
		using _to_native_handle_help_t = typename _to_native_handle_help_s<T>::peer_type;


		template<typename T>
		struct _to_generic_handle_help_s;

		template<typename T>
		using _to_generic_handle_help_t = typename _to_generic_handle_help_s<T>::peer_type;
	}
	template<typename T>
	static inline _to_native_handle_help_t<T> to_native_handle(T const p_handle) { return reinterpret_cast<_to_native_handle_help_t<T>>(p_handle.handle); } 

	template<typename T>
	static inline _to_generic_handle_help_t<T> to_generic_handle(T const p_handle) { return _to_generic_handle_help_t<T>{.handle=reinterpret_cast<void*>(p_handle)}; } 
} //namespace input
