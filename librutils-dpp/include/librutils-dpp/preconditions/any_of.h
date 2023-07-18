#pragma once

#include "precondition.h"
#include <vector>
#include <memory>

namespace rutils
{
	class any_of_t final : public precondition_t
	{
	public:
		explicit any_of_t(std::vector<precondition_t*>&& preconditions);

	protected:
		bool run_evaluate(dpp::cluster& cluster, const dpp::slashcommand_t& event) override;

	private:
		std::vector<std::unique_ptr<precondition_t>> preconditions;
	};
}
