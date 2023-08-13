#include "preconditions/any_of.h"

rutils::any_of_t::any_of_t(std::vector<base_precondition_t*>&& preconditions)
{
	this->preconditions.reserve(preconditions.size());

	for (base_precondition_t* precondition : preconditions)
	{
		this->preconditions.push_back(std::unique_ptr<base_precondition_t>(precondition));
	}
}

bool rutils::any_of_t::run_evaluate(dpp::cluster& cluster, const dpp::slashcommand_t& event)
{
	for (const std::unique_ptr<base_precondition_t>& precondition : preconditions)
	{
		if (precondition->evaluate(cluster, event))
		{
			return true;
		}
	}

	return false;
}
