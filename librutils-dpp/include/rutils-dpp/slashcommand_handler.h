#pragma once
#include <dpp/cluster.h>
#include <dpp/commandhandler.h>

#include "slashcommand_groups.h"


namespace rutils
{
	class slashcommand_handler_t final
	{
	public:
		explicit slashcommand_handler_t(dpp::cluster& bot);

		slashcommand_group_t& add_command_group(const std::string& name, const std::string& description, std::unique_ptr<precondition_t>&& precondition = nullptr, dpp::snowflake guild_id = 0);
		slashcommand_handler_t& add_command(const std::string& name, const std::string& description, dpp::parameter_registration_t&& parameters = {}, std::unique_ptr<precondition_t>&& precondition = nullptr, command_function_t function = default_function, dpp::snowflake guild_id = 0);

		void register_commands();

	private:
		friend slashcommand_group_t& slashcommand_group_t::add_sub_command(const std::string& name, const std::string& description, dpp::parameter_registration_t&& parameters, std::unique_ptr<precondition_t>&& precond, command_function_t function);
		friend slashcommand_sub_group_t& slashcommand_sub_group_t::add_sub_command(const std::string& name, const std::string& description, dpp::parameter_registration_t&& parameters, std::unique_ptr<precondition_t>&& precond, command_function_t function);
		static void add_parameter_options(std::vector<dpp::command_option>& options, const dpp::parameter_registration_t& parameters);

		void on_slashcommand(const dpp::slashcommand_t& event);

		void resolve_slashcommand
		(
			std::vector<dpp::command_data_option>* p_command_data, command_function_t** p_command_function, 
			std::vector<precondition_t*>& preconditions, const dpp::slashcommand_t& event
		);

		dpp::command_parameter resolve_slashcommand_parameter(const dpp::slashcommand_t& event, const dpp::command_data_option& data_option);
		static dpp::command_option_type get_command_option_type(dpp::parameter_type param_type);

		dpp::cluster& bot;

		std::list<slashcommand_t> commands{};
		std::list<slashcommand_group_t> command_groups{};
	};
}



