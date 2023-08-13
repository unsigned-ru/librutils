#include "slashcommand_handler.h"

#include <loggeru.h>


rutils::slashcommand_handler_t::slashcommand_handler_t(dpp::cluster& bot)
	: bot(bot)
{
	bot.on_slashcommand([this](const dpp::slashcommand_t& slashcommand) { this->on_slashcommand(slashcommand); });
}

rutils::slashcommand_group_t& rutils::slashcommand_handler_t::add_command_group(const std::string& name, const std::string& description, std::unique_ptr<base_precondition_t>&& precondition, dpp::snowflake guild_id)
{
	slashcommand_group_t command_group
	(
		std::move(precondition), dpp::slashcommand(name, description, bot.me.id), guild_id
	);

	return command_groups.emplace_back(std::move(command_group));
}

rutils::slashcommand_handler_t& rutils::slashcommand_handler_t::add_command(const std::string& name, const std::string& description, dpp::parameter_registration_t&& parameters, std::unique_ptr<base_precondition_t>&& precondition, command_function_t function, dpp::snowflake guild_id)
{
	//setup slashcommand
	slashcommand_t slashcommand(std::move(precondition), dpp::slashcommand(name, description, bot.me.id), guild_id, std::move(function));


	add_parameter_options(slashcommand.command.options, parameters);

	commands.emplace_back(std::move(slashcommand));
	return *this;
}

void rutils::slashcommand_handler_t::register_commands()
{
	std::map<dpp::snowflake, std::vector<dpp::slashcommand>> guild_commands{};
	std::vector<dpp::slashcommand> global_commands{};


	for (slashcommand_t& cmd : commands)
	{
		if (static_cast<uint64_t>(cmd.guild_id) == 0u)
		{
			global_commands.emplace_back(cmd.command);
		}
		else
		{
			if (guild_commands.contains(cmd.guild_id) == false)
			{
				//add the guild to the list
				guild_commands.insert(std::make_pair(cmd.guild_id, std::vector<dpp::slashcommand>{}));
			}
			guild_commands.at(cmd.guild_id).push_back(cmd.command);
		}
		
	}

	for (slashcommand_group_t& cmd_group : command_groups)
	{
		if (static_cast<uint64_t>(cmd_group.command.guild_id) == 0u)
		{
			global_commands.emplace_back(cmd_group.command.command);
		}
		else
		{
			if (guild_commands.contains(cmd_group.command.guild_id) == false)
			{
				//add the guild to the list
				guild_commands.insert(std::make_pair(cmd_group.command.guild_id, std::vector<dpp::slashcommand>{}));
			}
			guild_commands.at(cmd_group.command.guild_id).push_back(cmd_group.command.command);
		}
	}


	for (std::pair<const dpp::snowflake, std::vector<dpp::slashcommand>>& pair : guild_commands)
	{
		bot.guild_bulk_command_create(pair.second, pair.first);
	}

	bot.global_bulk_command_create(global_commands);
	
}

void rutils::slashcommand_handler_t::add_parameter_options(std::vector<dpp::command_option>& options, const dpp::parameter_registration_t& parameters)
{
	//loop over all parameters in the parameter_registration_t
	for (const auto& [param_name, param_info] : parameters)
	{
		//create an option from the parameter
		dpp::command_option& option = options.emplace_back(get_command_option_type(param_info.type), param_name, param_info.description, !param_info.optional);

		//add parameter choices to new option.
		if (param_info.choices.empty() == false)
		{
			for (const auto& [choice_value, choice_name] : param_info.choices)
			{
				option.add_choice(dpp::command_option_choice(choice_name, choice_value));
			}
		}
	}
}

void rutils::slashcommand_handler_t::on_slashcommand(const dpp::slashcommand_t& event)
{
	std::vector<dpp::command_data_option> p_command_data;
	command_function_t* p_command_function = nullptr;
	std::vector<base_precondition_t*> preconditions{};

	resolve_slashcommand(&p_command_data, &p_command_function, preconditions, event);

	if(p_command_function)
	{
		//run preconditions
		for(base_precondition_t* p_precondition : preconditions)
		{
			if (p_precondition && p_precondition->evaluate(bot, event) == false)
			{
				//failed to pass precondition, let the issuer know.
				event.reply(dpp::message("You do not have the required precondition(s) to execute this command!"));
				return;
			}
		}

		//create a parameter list
		dpp::parameter_list_t parameters{};
		for (const dpp::command_data_option& option : p_command_data)
		{
			parameters.emplace_back(option.name, resolve_slashcommand_parameter(event, option));
		}

		//run the command
		(*p_command_function)(parameters, event);
	}
}

void rutils::slashcommand_handler_t::resolve_slashcommand(std::vector<dpp::command_data_option>* p_command_data, command_function_t** p_command_function, std::vector<base_precondition_t*>& preconditions, const dpp::slashcommand_t& event)
{
	dpp::interaction interaction = event.command;
	dpp::command_interaction cmd_interaction = interaction.get_command_interaction();

	//get the command associated with the command_name
	const auto command_it = std::find_if
	(
		commands.begin(), commands.end(),
		[&interaction](const slashcommand_t& slashcommand)
		{
			return slashcommand.command.name == interaction.get_command_name();
		}
	);

	if (command_it != commands.end())
	{
		*p_command_data = cmd_interaction.options;
		*p_command_function = &command_it->function;
		preconditions.push_back(command_it->precondition.get());
		return;
	}
	//---------------------------

	//if we didn't find any simple command with that name, try and find a group with that name.
	const auto command_group_it = std::find_if
	(
		command_groups.begin(),
		command_groups.end(),
		[&interaction](const slashcommand_group_t& command_group)
		{
			return command_group.command.command.name == interaction.get_command_name();
		}
	);

	if (command_group_it != command_groups.end())
	{
		//loop variables
		slashcommand_subgroupable_i* p_command_sub_group = &*command_group_it;
		std::vector<dpp::command_data_option>* command_data_options = &cmd_interaction.options;

		//we found a sub_group with that command name.
		while (true)
		{
			preconditions.push_back(p_command_sub_group->precondition.get());

			//if no options are provided for group, return and don't reply
			if (command_data_options->empty())
			{
				//TODO: log something
				return;
			}

			//check if the option is a sub_command_group
			if ((*command_data_options)[0].type == dpp::co_sub_command_group)
			{
				auto sub_group_co_it = std::find_if
				(
					p_command_sub_group->command_sub_groups.begin(),
					p_command_sub_group->command_sub_groups.end(),
					[command_data_options](const slashcommand_sub_group_t& sub_group)
					{
						return sub_group.command_group.name == (*command_data_options)[0].name;
					}
				);

				if (sub_group_co_it == p_command_sub_group->command_sub_groups.end())
				{
					//if we didn't find the sub_group_command_option, then return and don't reply.
					//something went amiss.
					//TODO: log something
					return;
				}

				//we did find the sub_command_group
				//update loop variables
				p_command_sub_group = &*sub_group_co_it;
				command_data_options = &(*command_data_options)[0].options;

				//continue while loop -- in depth -- until we find a sub_command.
			}

			//check if the option is a sub_command
			if ((*command_data_options)[0].type == dpp::co_sub_command)
			{
				//find the correct sub-command from the command_options
				const auto sub_command_option_it = std::find_if
				(
					p_command_sub_group->command_sub_commands.begin(),
					p_command_sub_group->command_sub_commands.end(),
					[command_data_options](const slashcommand_sub_command_t& sub_command)
					{
						return sub_command.sub_command_option.name == (*command_data_options)[0].name;
					}
				);

				//if we didn't find the correct sub_command
				if (sub_command_option_it == p_command_sub_group->command_sub_commands.end())
				{
					//return and don't reply.
					return;
				}

				//we found the correct sub_command!
				//set the expected variables
				*p_command_data = (*command_data_options)[0].options;
				*p_command_function = &sub_command_option_it->function;
				preconditions.push_back(sub_command_option_it->precondition.get());
				return;
			}
		}
	}
}

dpp::command_parameter rutils::slashcommand_handler_t::resolve_slashcommand_parameter(const dpp::slashcommand_t& event, const dpp::command_data_option& data_option)
{
	switch (data_option.type)
	{
		case dpp::co_string:
		{
			return std::get<std::string>(data_option.value);
		}
		case dpp::co_integer:
		{
			return std::get<std::int64_t>(data_option.value);
		}
		case dpp::co_number:
		{
			return std::get<double>(data_option.value);
		}
		case dpp::co_boolean:
		{
			return std::get<bool>(data_option.value);
		}
		case dpp::co_user:
		{
			const dpp::snowflake uid = std::get<dpp::snowflake>(data_option.value);

			if
				(
					event.command.resolved.users.contains(uid)
					&&
					event.command.resolved.members.contains(uid)
					)
			{
				/* Fill in both member and user info */
				dpp::resolved_user resolved_user;
				resolved_user.member = event.command.resolved.members.at(uid);
				resolved_user.user = event.command.resolved.users.at(uid);
				return resolved_user;
			}

			LOG_CRITICAL_("Could find resolved user with uid '{}' in interaction resolved fields", static_cast<uint64_t>(uid));
			return nullptr;
		}
		case dpp::co_channel:
		{
			const dpp::snowflake cid = std::get<dpp::snowflake>(data_option.value);
			if (event.command.resolved.channels.contains(cid))
			{
				return event.command.resolved.channels.at(cid);
			}

			LOG_CRITICAL_("Could find resolved channel with cid '{}' in interaction resolved fields", static_cast<uint64_t>(cid));
			return nullptr;
		}
		case dpp::co_role:
		{
			const dpp::snowflake rid = std::get<dpp::snowflake>(data_option.value);

			if (event.command.resolved.roles.contains(rid))
			{
				return event.command.resolved.roles.at(rid);
			}

			LOG_CRITICAL_("Could find resolved role with rid '{}' in interaction resolved fields", static_cast<uint64_t>(rid));
			return nullptr;
		}
		default:
		{
			LOG_WARNING("Unknown command option type, may be due to lack of maintenance or changed to the api");
			return nullptr;
		}
	}
}


dpp::command_option_type rutils::slashcommand_handler_t::get_command_option_type(dpp::parameter_type param_type)
{
	switch (param_type)
	{
	case dpp::pt_boolean:
		return dpp::co_boolean;
	case dpp::pt_integer:
		return dpp::co_integer;
	default:
		LOG_WARNING("Unknown parameter option type, must be a deprecation or lack of maintenance of this function.");
		[[fallthrough]];
	case dpp::pt_string:
		return dpp::co_string;
	case dpp::pt_user:
		return dpp::co_user;
	case dpp::pt_role:
		return dpp::co_role;
	case dpp::pt_channel:
		return dpp::co_channel;
	case dpp::pt_double:
		return dpp::co_number;
	}
}

