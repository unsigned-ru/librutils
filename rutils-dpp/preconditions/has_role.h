#pragma once
#include "precondition.h"

namespace rutils
{
	class has_role_t final : public precondition_t
	{
	public:
		explicit has_role_t(dpp::snowflake role_id);

	protected:
		bool run_evaluate(dpp::cluster& cluster, const dpp::slashcommand_t& event) override;

	private:
		dpp::snowflake role_id;
	};
}
