//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///		Main entry point
///
///	\copyright
///		Copyright (c) Kaotic Software
///		
//======== ======== ======== ======== ======== ======== ======== ========

#include <filesystem>

#include <Windows.h>

#include <CoreLib/Core_StackTrace.hpp>
#include <CoreLib/Core_OS.hpp>
#include <CoreLib/toPrint/toPrint_filesystem.hpp>

#include <Logger/Logger_service.hpp>
#include <Logger/Logger.hpp>
#include <Logger/sink/log_file_sink.hpp>
#include <Logger/sink/log_debugger_sink.hpp>

#include <pathfinder/pathfinder.hpp>
#include <pathfinder/pathfinder_service.hpp>
#include <pathfinder/pathfinder_prelog_store.hpp>

#include "WindowApp.hpp"


#define LOG_TO_FILE 0

class pathfinder_Log_store: public pathfinder::Log_store
{
public:
	~pathfinder_Log_store()
	{
		while(!m_data.empty())
		{
			const pathfinder::Log_store::data_t& log_point = m_data.front();
			LOG_CUSTOM(log_point.file, log_point.line, log_point.column, log_point.level, log_point.message);
			m_data.pop();
		}
	}
};

int WINAPI wWinMain(
	[[maybe_unused]] HINSTANCE hInstance,
	[[maybe_unused]] HINSTANCE hPrevInstance,
	[[maybe_unused]] PWSTR pCmdLine,
	[[maybe_unused]] int nCmdShow)
{
	logger::log_debugger_sink runtime_debugger_log;
	logger::log_add_sink(runtime_debugger_log);

	core::disable_critical_invalid_c_param();

#if LOG_TO_FILE
	logger::log_file_sink     file_logger;
#endif

	const std::filesystem::path appDir = core::application_path().lexically_normal().parent_path();

	{
		pathfinder_Log_store log_store;
		{
			if(!pathfinder::load_pathfinder((appDir / "pathfinder.scef").lexically_normal(), log_store))
			{
				return -1;
			}
		}
		const std::filesystem::path& log_path = pathfinder::path_find(u8"log"sv);

		if(log_path.empty())
		{
			LOG_ERROR("Pathfinder does not define \"log\" path"sv);
		}
		else
		{
			core::register_crash_trace(log_path / "KaosEngine.strace");
#if LOG_TO_FILE
			std::error_code ec;
			std::filesystem::create_directories(log_path, ec);
			if(ec != std::error_code{})
			{
				LOG_ERROR("Failed to create Log directory \""sv, log_path, "\" errc: "sv, ec.message());
			}
			else
			{
				if(file_logger.init(log_path / "KaosEngine.log"))
				{
					logger::log_add_sink(file_logger);
				}
			}
#endif
		}
	}

	LOG_INFO("Pre setup complete"sv);

	int result = 0;
	{
		window_app application_obj;
		result = application_obj.run(hInstance);
	}

	pathfinder::clear_pathfinder();
	logger::log_remove_all();

	return result;
}
