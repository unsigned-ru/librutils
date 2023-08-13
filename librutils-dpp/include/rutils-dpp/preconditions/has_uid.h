#pragma once

#include "base_precondition.h"

namespace rutils
{
	class has_uid_t final : public base_precondition_t
	{
	public:
		explicit has_uid_t(dpp::snowflake user_id);

	protected:
		bool run_evaluate(dpp::cluster& cluster, const dpp::slashcommand_t& event) override;

	private:
		dpp::snowflake user_id;
	};
}