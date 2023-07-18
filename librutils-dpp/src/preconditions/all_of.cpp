#include "preconditions/all_of.h"

rutils::all_of_t::all_of_t(std::vector<precondition_t*>&& preconditions)
{
	this->preconditions.reserve(preconditions.size());

	for (precondition_t* precondition : preconditions)
	{
		this->preconditions.push_back(std::unique_ptr<precondition_t>(precondition));
	}
}

bool rutils::all_of_t::run_evaluate(dpp::cluster& cluster, const dpp::slashcommand_t& event)
{
	for (const std::unique_ptr<precondition_t>& precondition : preconditions)
	{
		if (precondition->evaluate(cluster, event) == false)
		{
			return false;
		}
	}

	return true;
}