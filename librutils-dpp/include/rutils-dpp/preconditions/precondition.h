#pragma once
#include <dpp/dispatcher.h>

namespace rutils
{
	class precondition_t
	{
	public:
		precondition_t() = default;
		virtual ~precondition_t() = default;

		bool evaluate(dpp::cluster& cluster, const dpp::slashcommand_t& event);

	protected:
		virtual bool run_evaluate(dpp::cluster& cluster, const dpp::slashcommand_t& event) = 0;
	};
}
