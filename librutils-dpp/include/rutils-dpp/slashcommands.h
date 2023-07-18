#pragma once
#include <loggeru.h>
#include <dpp/commandhandler.h>
#include "preconditions/precondition.h"


namespace rutils
{
	inline void default_function(const dpp::parameter_list_t&, const dpp::slashcommand_t& command)
	{
		LOG_INFO_("default_function called by command: {}.", command.command.get_command_name());
	};

	using command_function_t = std::function<void(const dpp::parameter_list_t&, const dpp::slashcommand_t&)>;

	struct slashcommand_t final
	{
		slashcommand_t(std::unique_ptr<precondition_t>&& precondition, dpp::slashcommand&& command, const dpp::snowflake guild_id, command_function_t function = default_function)
			: precondition(std::move(precondition))
			, command(command)
			, guild_id(guild_id)
			, function(std::move(function))
		{}

		std::unique_ptr<precondition_t> precondition;
		dpp::slashcommand command;
		dpp::snowflake guild_id;
		command_function_t function;
	};


	struct slashcommand_sub_command_t final
	{
		slashcommand_sub_command_t(std::unique_ptr<precondition_t>&& precondition, dpp::command_option& sub_command_option, command_function_t function = default_function)
			: precondition(std::move(precondition))
			, sub_command_option(sub_command_option)
			, function(std::move(function))
		{}

		std::unique_ptr<precondition_t> precondition;
		dpp::command_option& sub_command_option;
		command_function_t function;
	};

}