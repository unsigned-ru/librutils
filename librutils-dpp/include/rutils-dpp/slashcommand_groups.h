#pragma once
#include <list>
#include <memory>

#include "slashcommands.h"

namespace rutils
{
	//forward declarations
	struct slashcommand_sub_group_t;


	struct slashcommand_subgroupable_i
	{
		explicit slashcommand_subgroupable_i(std::unique_ptr<rutils::base_precondition_t>&& precondition);
		virtual ~slashcommand_subgroupable_i() = default;
		slashcommand_subgroupable_i(const slashcommand_subgroupable_i&) = delete;
		slashcommand_subgroupable_i& operator=(slashcommand_subgroupable_i&&) noexcept = delete;
		slashcommand_subgroupable_i(slashcommand_subgroupable_i&&) noexcept = default;

		std::unique_ptr<base_precondition_t> precondition;
		std::list<slashcommand_sub_group_t> command_sub_groups{};
		std::list<slashcommand_sub_command_t> command_sub_commands{};
	};

	struct slashcommand_group_t final : slashcommand_subgroupable_i
	{
		explicit slashcommand_group_t(std::unique_ptr<base_precondition_t>&& precond, dpp::slashcommand&& command, dpp::snowflake guild_id, command_function_t function = default_function);

		slashcommand_t command;

		slashcommand_group_t& add_sub_command(const std::string& name, const std::string& description, dpp::parameter_registration_t&& parameters = {}, std::unique_ptr<base_precondition_t>&& precond = nullptr, command_function_t function = default_function);
		slashcommand_sub_group_t& add_command_sub_group(const std::string& name, const std::string& description, std::unique_ptr<base_precondition_t>&& precond = nullptr);
	};

	struct slashcommand_sub_group_t final : slashcommand_subgroupable_i
	{

		explicit slashcommand_sub_group_t(std::unique_ptr<base_precondition_t>&& precond, dpp::command_option& command_group);

		dpp::command_option& command_group;

		slashcommand_sub_group_t& add_sub_command(const std::string& name, const std::string& description, dpp::parameter_registration_t&& parameters = {}, std::unique_ptr<base_precondition_t>&& precond = nullptr, command_function_t function = default_function);
		slashcommand_sub_group_t& add_command_sub_group(const std::string& name, const std::string& description, std::unique_ptr<base_precondition_t>&& preconditionn);
	};
}

