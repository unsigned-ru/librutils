#include "has_permissions.h"
#include "../utils.h"

rutils::has_permissions_t::has_permissions_t(dpp::permissions permissions)
	: permissions(permissions)
{}

bool rutils::has_permissions_t::run_evaluate(dpp::cluster& cluster, const dpp::slashcommand_t& event)
{
	//get the issueing member
	const dpp::guild_member& guild_member = event.command.member;
	return rutils::member_has_guild_permissions(cluster, guild_member, permissions);
}
