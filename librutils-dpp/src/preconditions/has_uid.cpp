#include "preconditions/has_uid.h"

rutils::has_uid_t::has_uid_t(dpp::snowflake user_id)
	: user_id(user_id)
{}

bool rutils::has_uid_t::run_evaluate(dpp::cluster& /*cluster*/, const dpp::slashcommand_t& event)
{
	return event.command.usr.id == user_id;
}
