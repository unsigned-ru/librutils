#pragma once
#include <loggeru.h>
#include <string>
#include <dpp/dpp.h>

namespace rutils
{
	std::string get_user_display_name(const dpp::user& user);

	bool member_has_guild_role(dpp::cluster& bot, const dpp::snowflake& guild_id, const dpp::snowflake& member_id, const dpp::snowflake& role_id);
	bool member_has_guild_role(const dpp::guild_member& member, const dpp::snowflake& role_id);

	std::vector<dpp::role> member_get_guild_roles(dpp::cluster& bot, const dpp::snowflake& guild_id, const dpp::snowflake& member_id);
	std::vector<dpp::role> member_get_guild_roles(dpp::cluster& bot, const dpp::guild_member& member);

	dpp::permission member_get_guild_permissions(dpp::cluster& bot, const dpp::snowflake& guild_id, const dpp::snowflake& member_id);
	dpp::permission member_get_guild_permissions(dpp::cluster& bot, const dpp::guild_member& member);

	bool member_has_guild_permissions(dpp::cluster& bot, const dpp::snowflake& guild_id, const dpp::snowflake& member_id, dpp::permission permissions);
	bool member_has_guild_permissions(dpp::cluster& bot, const dpp::guild_member& member, dpp::permission permissions);

	template<typename out_t>
	const out_t* find_optional_param(const dpp::parameter_list_t& params, const std::string& param_name)
	{
		auto it = std::find_if(params.begin(), params.end(), [&param_name](const std::pair<std::string, dpp::command_parameter>& pair) { return pair.first == param_name; });

		if (it == params.end())
			return nullptr;

		return &std::get<out_t>(it->second);
	}

	template<typename out_t>
	const out_t& find_param(const dpp::parameter_list_t& params, const uint32_t param_idx)
	{
		return std::get<out_t>(params.at(param_idx).second);
	}

	template<typename out_t>
	const out_t& find_param(const dpp::parameter_list_t& params, const std::string& param_name)
	{
		auto it = std::find_if(params.begin(), params.end(), [&param_name](const std::pair<std::string, dpp::command_parameter>& pair) { return pair.first == param_name; });

		if (it == params.end())
			LOG_ERROR("Could not find required parameter.");

		return *it;
	}

	dpp::component* depth_first_search_component_by_custom_id(uint32_t interaction_id, std::vector<dpp::component>& components);
	const dpp::component* depth_first_search_component_by_custom_id(uint32_t interaction_id, const std::vector<dpp::component>& components);


	void set_default_option_by_value(std::vector<dpp::select_option>& selection_options, const std::string& value);

	uint64_t get_random_seed(uint8_t seed_length = 7);

	std::string& set_code_line(std::string& string);
	std::string set_code_line(const std::string& string);

	std::string& set_code_block(std::string& string);
	std::string set_code_block(const std::string& string);

}
