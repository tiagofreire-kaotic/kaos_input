//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once


#include <kaos_input_backend/kaos_input_backend_api.h>


#include <LogLib/logger_struct.hpp>

#include <CoreLib/string/core_os_string.hpp>
#include <CoreLib/toPrint/toPrint.hpp>
#include <CoreLib/core_module.hpp>


namespace kaos::input::backend
{

///	\brief Public interface for logging
kaos_input_backend_API void log_message(::logger::log_message_data const& data, std::u8string_view message);

namespace _p
{
	///	\brief Public interface for log filtering
	[[nodiscard]] kaos_input_backend_API bool log_check_filter(::logger::log_filter_data const& data);

	class LogStreamer: public core::sink_toPrint_base
	{
	private:
		::logger::log_message_data const& m_data;

	public:
		constexpr inline LogStreamer(
			::logger::log_message_data const& data)
			: m_data	(data)
		{
		}

		void write(const std::u8string_view p_message) const
		{
			::kaos::input::backend::log_message(m_data, p_message);
		}
	};

	inline constexpr void no_op() {}

} //namespace _p

} //namespace kaos::input::backend


//======== ======== Macro Magic ======== ========

#ifdef _WIN32
#define __LOG_FILE __FILEW__

#else
#define __LOG_FILE __FILE__
#endif

#define LOG_CUSTOM(File, Line, Column, _Level, ...) \
	{ \
		::logger::log_message_data _P_BASE_LOG_DATA; \
		_P_BASE_LOG_DATA.module_base = ::core::get_current_module_base(); \
		_P_BASE_LOG_DATA.module_name = ::core::get_current_module_name(); \
		_P_BASE_LOG_DATA.file        = ::core::os_string_view{__LOG_FILE}; \
		_P_BASE_LOG_DATA.line        = static_cast<uint32_t>(__LINE__); \
		_P_BASE_LOG_DATA.column      = 0; \
		_P_BASE_LOG_DATA.level       = _Level; \
		if(::kaos::input::backend::_p::log_check_filter(_P_BASE_LOG_DATA)) \
		{ \
			_P_BASE_LOG_DATA.file   = File; \
			_P_BASE_LOG_DATA.line   = Line; \
			_P_BASE_LOG_DATA.column = Column; \
			core::print<char8_t>(::kaos::input::backend::_p::LogStreamer(_P_BASE_LOG_DATA) __VA_OPT__(,) __VA_ARGS__); \
		} \
	}


/// \brief Helper Macro to assist on message formating and automatically filling of __FILE__ (__FILEW__ on windows) and __LINE__
/// \param[in] Level - \ref logger::Level
#define LOG_MESSAGE(Level, ...) LOG_CUSTOM(::core::os_string_view{__LOG_FILE}, static_cast<uint32_t>(__LINE__), 0, Level, __VA_ARGS__)

/// \brief Helper Macro for info logs
#define LOG_INFO(...)		LOG_MESSAGE(::logger::Level::Info, __VA_ARGS__)

/// \brief Helper Macro for warning logs
#define LOG_WARNING(...)	LOG_MESSAGE(::logger::Level::Warning, __VA_ARGS__)

/// \brief Helper Macro for error logs
#define LOG_ERROR(...)		LOG_MESSAGE(::logger::Level::Error, __VA_ARGS__)

#ifdef _DEBUG
/// \brief Helper Macro for debug logs. If built in debug mode, message will be logged, if built in release the Macro will automatically eliminate the lines of code.
#	define LOG_DEBUG(...)	LOG_MESSAGE(::logger::Level::Debug, __VA_ARGS__)
#else
/// \brief Helper Macro for debug logs. If built in debug mode, message will be logged, if built in release the Macro will automatically eliminate the lines of code.
#	define LOG_DEBUG(...)	::kaos::input::backend::_p::no_op();
#endif
