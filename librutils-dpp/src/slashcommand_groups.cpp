#include "slashcommand_groups.h"
#include "slashcommand_handler.h"


rutils::slashcommand_subgroupable_i::slashcommand_subgroupable_i(std::unique_ptr<precondition_t>&& precondition)
	: precondition(std::move(precondition))
{
}

rutils::slashcommand_group_t::slashcommand_group_t(std::unique_ptr<precondition_t>&& precond, dpp::slashcommand&& command, dpp::snowflake guild_id, command_function_t function)
	: slashcommand_subgroupable_i(std::move(precond))
	, command(nullptr, std::move(command), guild_id, std::move(function))
{
}

rutils::slashcommand_group_t& rutils::slashcommand_group_t::add_sub_command
(
	const std::string& name, const std::string& description,
	dpp::parameter_registration_t&& parameters, std::unique_ptr<precondition_t>&& precond,
	command_function_t function
)
{
	//add a sub-command to the group.
	command.command.options.reserve(10);
	dpp::command_option& sub_command_option = command.command.options.emplace_back(dpp::co_sub_command, name, description);

	slashcommand_handler_t::add_parameter_options(sub_command_option.options, parameters);
	command_sub_commands.emplace_back(std::move(precond), sub_command_option, std::move(function));

	return *this;
}

rutils::slashcommand_sub_group_t& rutils::slashcommand_group_t::add_command_sub_group
(
	const std::string& name, const std::string& description, std::unique_ptr<precondition_t>&& precond
)
{
	command.command.options.reserve(10);
	dpp::command_option& option = command.command.options.emplace_back(dpp::co_sub_command_group, name, description);

	return command_sub_groups.emplace_back(std::move(precond), option);
}

rutils::slashcommand_sub_group_t::slashcommand_sub_group_t(std::unique_ptr<precondition_t>&& precond, dpp::command_option& command_group)
	: slashcommand_subgroupable_i(std::move(precond))
	, command_group(command_group)
{}


// ---------------------------------------


rutils::slashcommand_sub_group_t& rutils::slashcommand_sub_group_t::add_sub_command
(
	const std::string& name, const std::string& description, dpp::parameter_registration_t&& parameters, std::unique_ptr<precondition_t>&& precond, command_function_t function
)
{
	command_group.options.reserve(10);
	//add a sub-command to the sub-group.
	dpp::command_option& sub_command_option = command_group.options.emplace_back(dpp::co_sub_command, name, description);

	slashcommand_handler_t::add_parameter_options(sub_command_option.options, parameters);
	command_sub_commands.emplace_back(std::move(precond), sub_command_option, std::move(function));

	return *this;
}

rutils::slashcommand_sub_group_t& rutils::slashcommand_sub_group_t::add_command_sub_group
(
	const std::string& name, const std::string& description, std::unique_ptr<precondition_t>&& precond
)
{
	command_group.options.reserve(10);
	dpp::command_option& sub_command_group_option = command_group.options.emplace_back(dpp::co_sub_command_group, name, description);

	return command_sub_groups.emplace_back(std::move(precond), sub_command_group_option);
}


