#pragma once
#include "service.h"

#include <unordered_map>
#include <memory>
#include <type_traits>
#include <loggeru.h>

namespace rutils
{
	class service_locator_t final
	{
	public:
		template <typename service_type_t>
		static service_type_t& register_service() requires std::is_base_of_v<service_t, service_type_t>
		{
			return register_service(std::make_unique<service_type_t>());
		}

		template <typename service_type_t>
		static service_type_t& register_service(std::unique_ptr<service_type_t>&& p_service) requires std::is_base_of_v<service_t, service_type_t>
		{
			//check if we have the service yet
			const auto& type = typeid(service_type_t);
			const auto type_hash = type.hash_code();

			if (services.contains(type_hash) == true)
			{
				LOG_WARNING_("Service: {} is already registered. Returning that instance...", type.name());
				return get_service<service_type_t>();
			}

			LOG_INFO_("Registered service: {}", type.name());
			return *static_cast<service_type_t*>(services.emplace(type_hash, std::move(p_service)).first->second.get());
		}

		template <typename service_type_t>
		static service_type_t& get_service() requires std::is_base_of_v<service_t, service_type_t>
		{
			const auto it = services.find(typeid(service_type_t).hash_code());

			ASSERT_MSG(it != services.end(), "Tried getting unregistered service!");
			ASSERT_MSG((*it).second, "Service was nullptr. Service was likely deleted manually unexpectedly. Make sure you don't delete services through any other means then 'remove_service' or 'clear_services'");

			return *static_cast<service_type_t*>((*it).second.get());
		}

		template <typename service_type_t>
		static void remove_service() requires std::is_base_of_v<service_t, service_type_t>
		{
			const auto& type = typeid(service_type_t);
			const auto type_hash = type.hash_code();

			const auto it = services.find(type_hash);

			if (it == services.end())
			{
				LOG_WARNING_("Trying to remove unregistered service: {}\nSkipping request...", type.name());
			}

			services.erase(it);
		}

		static void clear_services()
		{
			services.clear();
		}

	private:
		explicit service_locator_t() = default;

		inline static std::unordered_map<size_t, std::unique_ptr<service_t>> services{};
	};
}
