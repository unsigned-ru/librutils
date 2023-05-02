#include "has_channel.h"

rutils::has_channel_t::has_channel_t(dpp::snowflake channel_id)
	: channel_id(std::move(channel_id))
{}

bool rutils::has_channel_t::run_evaluate(dpp::cluster& /*cluster*/, const dpp::slashcommand_t& event)
{
	return event.command.channel_id == channel_id;
}
