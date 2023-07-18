#include "preconditions/precondition.h"

bool rutils::precondition_t::evaluate(dpp::cluster& cluster, const dpp::slashcommand_t& event)
{
	return run_evaluate(cluster, event);
}
