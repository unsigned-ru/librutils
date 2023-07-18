#pragma once
#include <dpp/dpp.h>

#define LAMBDA_SLASH_COMMAND_FUNCTION(command_function)\
[this](const std::string& command, const dpp::parameter_list_t& parameters, dpp::command_source source)\
{\
	return command_utility_execute\
	(\
		command, parameters, source,\
		[this](const std::string& cmd, const dpp::parameter_list_t& params, dpp::command_source& src, bool& has_replied) \
		{\
			return command_function(cmd, params, src, has_replied);\
		}\
	); \
}

namespace rutils::discord
{
	class base_bot_t
	{
	public:
		explicit base_bot_t(const std::string& token, uint32_t intents = dpp::i_all_intents);
		virtual ~base_bot_t();

		base_bot_t(const base_bot_t&) = delete;
		base_bot_t(base_bot_t&&) noexcept = delete;
		base_bot_t& operator=(const base_bot_t&) = delete;
		base_bot_t& operator=(base_bot_t&&) noexcept = delete;

		void run(bool return_after_init = false);

	protected:
		virtual void run_impl(bool return_after_init);

		void command_utility_execute
		(
			const std::string& command, const dpp::parameter_list_t& params, dpp::command_source& source,
			const std::function<void(const std::string&, const dpp::parameter_list_t&, dpp::command_source&, bool&)>& command_logic
		);

		void command_utility_think(const dpp::command_source& source, bool& has_replied);

		dpp::cluster bot;
		dpp::commandhandler command_handler;


		virtual void on_ready(const dpp::ready_t& e);
		virtual void on_log(const dpp::log_t& log);
	};
}