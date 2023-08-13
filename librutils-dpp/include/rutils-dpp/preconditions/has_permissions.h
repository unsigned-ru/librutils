#pragma once
#include "base_precondition.h"

namespace rutils
{
	class has_permissions_t final : public base_precondition_t
	{
	public:
		explicit has_permissions_t(dpp::permissions permissions);

	protected:
		bool run_evaluate(dpp::cluster& cluster, const dpp::slashcommand_t& event) override;

	private:
		dpp::permissions permissions;
	};
}
