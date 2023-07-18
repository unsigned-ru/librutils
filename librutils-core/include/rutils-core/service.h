#pragma once

namespace rutils
{
	class service_t
	{
	public:
		virtual ~service_t() = default;

		explicit service_t(const service_t&) = delete;
		explicit service_t(service_t&&) noexcept = delete;

		service_t& operator=(const service_t&) = delete;
		service_t& operator=(service_t&&) noexcept = delete;

	protected:
		explicit service_t() = default;
	};
}
