#pragma once

#include "precondition.h"

namespace rutils
{
	class all_of_t final : public precondition_t
	{
	public:
		explicit all_of_t(std::vector<precondition_t*>&& preconditions);

	protected:
		bool run_evaluate(dpp::cluster& cluster, const dpp::slashcommand_t& event) override;

	private:
		std::vector<std::unique_ptr<precondition_t>> preconditions;
	};
}
