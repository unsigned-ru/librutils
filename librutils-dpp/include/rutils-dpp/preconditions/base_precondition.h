#pragma once
#include <dpp/dispatcher.h>

namespace rutils
{
	class base_precondition_t
	{
	public:
		base_precondition_t() = default;
		virtual ~base_precondition_t() = default;

		bool evaluate(dpp::cluster& cluster, const dpp::slashcommand_t& event);

	protected:
		virtual bool run_evaluate(dpp::cluster& cluster, const dpp::slashcommand_t& event) = 0;
	};
}
