#pragma once

#include "base_precondition.h"

namespace rutils
{
	class has_channel_t final : public base_precondition_t
	{
	public:
		explicit has_channel_t(dpp::snowflake channel_id);

	protected:
		bool run_evaluate(dpp::cluster& cluster, const dpp::slashcommand_t& event) override;

	private:
		dpp::snowflake channel_id;
	};
}
