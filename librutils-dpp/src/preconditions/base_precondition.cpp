#include "preconditions/base_precondition.h"

bool rutils::base_precondition_t::evaluate(dpp::cluster& cluster, const dpp::slashcommand_t& event)
{
	return run_evaluate(cluster, event);
}
