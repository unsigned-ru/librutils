#pragma once

#include "base_precondition.h"

namespace rutils
{
	class all_of_t final : public base_precondition_t
	{
	public:
		explicit all_of_t(std::vector<base_precondition_t*>&& preconditions);

	protected:
		bool run_evaluate(dpp::cluster& cluster, const dpp::slashcommand_t& event) override;

	private:
		std::vector<std::unique_ptr<base_precondition_t>> preconditions;
	};
}
