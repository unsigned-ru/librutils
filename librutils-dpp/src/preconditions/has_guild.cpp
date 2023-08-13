#include "preconditions/has_guild.h"

rutils::has_guild_t::has_guild_t(dpp::snowflake guild_id)
	: guild_id(guild_id)
{}

bool rutils::has_guild_t::run_evaluate(dpp::cluster& /*cluster*/, const dpp::slashcommand_t& event)
{
	return event.command.guild_id == guild_id;
}
