#include "base_bot.h"

#include "utils.h"

rutils::discord::base_bot_t::base_bot_t(const std::string& token, uint32_t intents)
	: bot{ token, intents }
	, command_handler(&bot)
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

void rutils::discord::base_bot_t::command_utility_execute
(
	const std::string& command, const dpp::parameter_list_t& params, dpp::command_source& source,
	const std::function<void(const std::string&, const dpp::parameter_list_t&, dpp::command_source&, bool&)>& command_logic
)
{
	bool has_replied = false;

	try
	{
		command_logic(command, params, source, has_replied);
	}
	catch (const std::exception& ex)
	{
		const std::string response_message = fmt::format("Something went wrong: {}", ex.what());

		if (has_replied == false)
		{
			command_handler.reply(response_message, source);
		}
		else
		{
			source.interaction_event->edit_response(response_message);
		}

		LOG_WARNING_("Command '{}' for user '{}' failed to execute REASON: {}", command, rutils::get_user_display_name(source.issuer), ex.what());
	}
}

void rutils::discord::base_bot_t::command_utility_think(const dpp::command_source& source, bool& has_replied)
{
	std::condition_variable cond;
	std::mutex m;

	//set bot to "thinking" to void long response time issues
	command_handler.thinking
	(
		source,
		[&cond](const dpp::confirmation_callback_t& cb)
		{
			if (cb.is_error())
			{
				LOG_ERROR_("Error setting the bot to 'thinking': {}", cb.get_error().message);
			}

			cond.notify_all();
		}
	);

	//wait for the "thinking" callback to complete
	std::unique_lock lock(m);
	cond.wait(lock);
	has_replied = true;
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