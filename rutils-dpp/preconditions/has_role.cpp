#include "has_role.h"

rutils::has_role_t::has_role_t(dpp::snowflake role_id)
	: role_id(role_id)
{}

bool rutils::has_role_t::run_evaluate(dpp::cluster& /*cluster*/, const dpp::slashcommand_t& event)
{
	//get the issueing member
	const dpp::guild_member& guild_member = event.command.member;

	//check if they have the role
	return std::find(guild_member.roles.begin(), guild_member.roles.end(), role_id) != guild_member.roles.end();
}
