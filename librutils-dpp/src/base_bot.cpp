#include "base_bot.h"

#include "utils.h"

rutils::discord::base_bot_t::base_bot_t(const std::string& token, uint32_t intents)
	: bot{ token, intents }
	, command_handler(bot)
{
	bot.on_ready.attach([this](const dpp::ready_t& ready) { on_ready(ready); });
	bot.on_log.attach([this](const dpp::log_t& log) { on_log(log); });
}

rutils::discord::base_bot_t::~base_bot_t() = default;

void rutils::discord::base_bot_t::run(const bool return_after_init)
{
	run_impl(return_after_init);
}

void rutils::discord::base_bot_t::run_impl(const bool return_after_init)
{
	bot.start(return_after_init);
}

void rutils::discord::base_bot_t::on_ready(const dpp::ready_t& /*e*/)
{
	LOG_INFO_("Logged in as {}!", bot.me.username);
}

void rutils::discord::base_bot_t::on_log(const dpp::log_t& log)
{
	switch (log.severity)
	{
	case dpp::ll_trace:
	case dpp::ll_debug:
		LOG_DEBUG(log.message);
		break;
	default:
		[[fallthrough]];
	case dpp::ll_info:
		LOG_INFO(log.message);
		break;
	case dpp::ll_warning:
		LOG_WARNING(log.message);
		break;
	case dpp::ll_error:
		LOG_WARNING_("DPP_ERROR: {}", log.message);
		break;
	case dpp::ll_critical:
		LOG_ERROR(log.message);
		break;
	}
}