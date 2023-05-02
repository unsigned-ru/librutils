#include "utils.h"
#include <sstream>
#include <iomanip>
#include <random>
#include <ranges>

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wrestrict"
#endif
std::string rutils::get_user_display_name(const dpp::user& user)
{
	std::stringstream ss{};
	ss << user.username << "#" << std::setw(4) << std::setfill('0') << user.discriminator;
	return ss.str();
}

bool rutils::member_has_guild_role(dpp::cluster& bot, const dpp::snowflake& guild_id, const dpp::snowflake& member_id, const dpp::snowflake& role_id)
{
	try
	{
		const dpp::guild_member member = bot.guild_get_member_sync(guild_id, member_id);
		return member_has_guild_role(member, role_id);
	}
	catch (const std::exception& ex)
	{
		LOG_WARNING_("member_has_guild_role > {}", ex.what());
	}

	return false;
}

bool rutils::member_has_guild_role(const dpp::guild_member& member, const dpp::snowflake& role_id)
{
	return std::ranges::find(member.roles, role_id) != member.roles.end();
}

std::vector<dpp::role> rutils::member_get_guild_roles(dpp::cluster& bot, const dpp::snowflake& guild_id, const dpp::snowflake& member_id)
{
	try
	{
		const dpp::guild_member member = bot.guild_get_member_sync(guild_id, member_id);
		return member_get_guild_roles(bot, member);
	}
	catch (const std::exception& ex)
	{
		LOG_WARNING_("member_get_guild_roles > {}", ex.what());
	}

	return {};
}

std::vector<dpp::role> rutils::member_get_guild_roles(dpp::cluster& bot, const dpp::guild_member& member)
{
	std::vector<dpp::role> member_roles{};

	try
	{
		dpp::role_map guild_role_map = bot.roles_get_sync(member.guild_id);

		//add all roles that the user has to the member_roles vector.
		for (const dpp::snowflake& role_id : member.roles)
		{
			if (auto it = guild_role_map.find(role_id); it != guild_role_map.end()) //O(1) search since it's a map.
			{
				member_roles.push_back(it->second);
			}
		}
	}
	catch (const std::exception& ex)
	{
		LOG_WARNING_("member_get_guild_roles > {}", ex.what());
	}

	return member_roles;
}

dpp::permission rutils::member_get_guild_permissions(dpp::cluster& bot, const dpp::snowflake& guild_id, const dpp::snowflake& member_id)
{
	try
	{
		//find member,
		const dpp::guild_member member = bot.guild_get_member_sync(guild_id, member_id);
		// then pass it along to the overloaded function
		return member_get_guild_permissions(bot, member);
	}
	catch (const std::exception& ex)
	{
		LOG_WARNING_("member_get_guild_permissions > {}", ex.what());
	}

	return 0u;
}

dpp::permission rutils::member_get_guild_permissions(dpp::cluster& bot, const dpp::guild_member& member)
{
	dpp::permission member_permissions{};

	const std::vector<dpp::role> member_roles = member_get_guild_roles(bot, member);

	for (const dpp::role& member_role : member_roles)
	{
		member_permissions |= member_role.permissions;
	}


	return member_permissions;
}

bool rutils::member_has_guild_permissions(dpp::cluster& bot, const dpp::snowflake& guild_id, const dpp::snowflake& member_id, dpp::permission permissions)
{
	try
	{
		//find member,
		const dpp::guild_member member = bot.guild_get_member_sync(guild_id, member_id);
		// then pass it along to the overloaded function
		return member_has_guild_permissions(bot, member, permissions);
	}
	catch (const std::exception& ex)
	{
		LOG_WARNING_("member_get_guild_permissions > {}", ex.what());
	}

	return false;
}

bool rutils::member_has_guild_permissions(dpp::cluster& bot, const dpp::guild_member& member, const dpp::permission permissions)
{
	dpp::permission member_permissions = member_get_guild_permissions(bot, member);

	return permissions == (member_permissions & permissions);
}



const dpp::component* rutils::depth_first_search_component_by_custom_id(const uint32_t interaction_id, const std::vector<dpp::component>& components)
{
	//DEPTH FIRST SEARCH ALGORITHM
	for (const dpp::component& component : components)
	{
		try
		{
			if (static_cast<uint32_t>(std::stoi(component.custom_id)) == interaction_id)
			{
				return &component;
			}
		}
		catch (const std::invalid_argument&) {}	//These exceptions are to be expected since not all custom_ids will be integral
		catch (const std::out_of_range&) {}		//These exceptions are to be expected since not all custom_ids will be integral

		//depth first search, recursion on it's components
		if (component.components.empty() == false) //if it does have components
		{
			const dpp::component* res = depth_first_search_component_by_custom_id(interaction_id, component.components);
			if (res != nullptr)
			{
				return res;
			}
		}
	}

	//we did not find any component matching the interaction id
	return nullptr;
}

dpp::component* rutils::depth_first_search_component_by_custom_id(const uint32_t interaction_id, std::vector<dpp::component>& components)
{
	//DEPTH FIRST SEARCH ALGORITHM
	for (dpp::component& component : components)
	{
		try
		{
			if (static_cast<uint32_t>(std::stoi(component.custom_id)) == interaction_id)
			{
				return &component;
			}
		}
		catch (const std::invalid_argument&) {}	//These exceptions are to be expected since not all custom_ids will be integral
		catch (const std::out_of_range&) {}		//These exceptions are to be expected since not all custom_ids will be integral

		//depth first search, recursion on it's components
		if (component.components.empty() == false) //if it does have components
		{
			dpp::component* res = depth_first_search_component_by_custom_id(interaction_id, component.components);
			if (res != nullptr)
			{
				return res;
			}
		}
	}

	//we did not find any component matching the interaction id
	return nullptr;
}

void rutils::set_default_option_by_value(std::vector<dpp::select_option>& selection_options, const std::string& value)
{
	for (dpp::select_option& option : selection_options)
	{
		option.set_default(option.value == value);
	}
}

uint64_t rutils::get_random_seed(uint8_t seed_length)
{
	std::default_random_engine rand_dev(static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::mt19937 generator(rand_dev());
	std::uniform_int_distribution<uint64_t> distr(0u, static_cast<uint64_t>(powf(10, seed_length)) - 1u);

	return distr(generator);
}

std::string& rutils::set_code_line(std::string& string)
{
	string = fmt::format("`{}`", string);
	return string;
}

std::string rutils::set_code_line(const std::string& string)
{
	return fmt::format("`{}`", string);
}

std::string& rutils::set_code_block(std::string& string)
{
	if (string.empty())
	{
		string = " ";
	}

	string = fmt::format("```{}```", string);
	return string;

}

std::string rutils::set_code_block(const std::string& string)
{
	return fmt::format("```{}```", string.empty() ? " " : string);
}


#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif