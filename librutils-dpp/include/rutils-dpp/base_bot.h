#pragma once
#include <dpp/dpp.h>

#include "slashcommand_handler.h"

#define LAMBDA_SLASH_COMMAND_FUNCTION(command_function)\
[this](const dpp::parameter_list_t& parameters, const dpp::slashcommand_t& event)\
{\
	return command_function(parameters, event);\
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

		dpp::cluster bot;
		slashcommand_handler_t command_handler;


		virtual void on_ready(const dpp::ready_t& e);
		virtual void on_log(const dpp::log_t& log);
	};
}