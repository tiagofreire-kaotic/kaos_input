//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///		
//======== ======== ======== ======== ======== ======== ======== ========

#include <kaos_input_services/logger.hpp>
#include <kaos_input_services/pathfinder.hpp>

#include <string>

#include <pathfinderLib/pathfinder.hpp>
#include <pathfinderLib/pathfinder_prelog_store.hpp>

#include <LogLib/logger_struct.hpp>
#include <LogLib/log_filter.hpp>
#include <LogLib/logger_group.hpp>

#include <LogLib/sink/log_debugger_sink.hpp>

#include <CoreLib/toPrint/toPrint_filesystem.hpp>
#include <CoreLib/core_os.hpp>
#include <CoreLib/core_module.hpp>

namespace kaos_input_services
{
	namespace
	{


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


		class ServiceManager
		{
		public:

			bool init()
			{
				m_logger.add_sink(m_debug_log);

				std::filesystem::path const module_dir = std::filesystem::path{core::get_current_module_name()}.parent_path();
				{
					pathfinder_Log_store log_store;
					{
						std::filesystem::path const config_path = (module_dir / "kaos_input.scef");
						if(!m_pathfinder.load(config_path, log_store))
						{
							LOG_ERROR("Failed to load pathfinder config \""sv, config_path, '\"');
						}
					}
				}

			}

			~ServiceManager()
			{
				m_logger.clear();
				m_pathfinder.clear();
			}

			pathfinder::PathFinder m_pathfinder;
			logger::LoggerGroup m_logger;

			logger::log_filter const* m_filter = nullptr;
			bool m_default_filter_behaviour = true;

		private:
#ifdef _WIN32
			logger::log_debugger_sink m_debug_log;
#endif
		};

		ServiceManager g_manager;

	} //namespace





	const std::filesystem::path& path_find(std::u8string_view const p_category)
	{
		return g_manager.m_pathfinder.get_path(p_category);
	}

	void log_message(::logger::log_message_data const& data, std::u8string_view const message)
	{
		g_manager.m_logger.log(data, message);
	}

	namespace _p
	{
		[[nodiscard]] bool log_check_filter(::logger::log_filter_data const& data)
		{
			if(g_manager.m_filter)
			{
				return g_manager.m_filter->filter(data);
			}
			return g_manager.m_default_filter_behaviour;
		}
	} //namespace _p
} //namespace kaos_input_services
