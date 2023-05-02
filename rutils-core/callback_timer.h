#pragma once
#include <chrono>
#include <thread>
#include <functional>

namespace rutils
{
	class callback_timer_t final
	{

	public:
		callback_timer_t() = default;
		callback_timer_t(std::function<void(void)>&& func, const int32_t interval, bool should_repeat = false, bool auto_start = true);
		template <class chrono_duration_t>
		callback_timer_t(std::function<void(void)>&& func, const chrono_duration_t& interval, bool should_repeat = false, bool auto_start = true)
			: callback_timer_t(std::move(func), std::chrono::duration_cast<std::chrono::milliseconds>(interval), should_repeat, auto_start)
		{}
		callback_timer_t(std::function<void(void)>&& func, const std::chrono::milliseconds& interval, bool should_repeat = false, bool auto_start = true);
		~callback_timer_t();


		void start();
		void stop();
		void restart();

		bool get_is_elapsed() const;
		bool is_runnning() const;
		bool is_repeating() const;


		callback_timer_t* set_function(std::function<void(void)>&& func);
		callback_timer_t* set_interval(const uint32_t interval);
		callback_timer_t* set_repeating(const bool should_repeat);

		template <class chrono_duration_t>
		chrono_duration_t get_interval() const
		{
			return std::chrono::duration_cast<chrono_duration_t>(interval);
		}


		template <class chrono_duration_t>
		chrono_duration_t get_time_to_next_tick() const
		{
			return std::chrono::duration_cast<chrono_duration_t>(get_time_since_last_tick<chrono_duration_t>() - interval);
		}

		template <class chrono_duration_t>
		chrono_duration_t get_time_since_last_tick() const
		{
			return std::chrono::duration_cast<chrono_duration_t>(std::chrono::system_clock::now() - latest_tick_time_point);
		}

		std::chrono::time_point<std::chrono::system_clock> get_next_tick_time_point() const;
		std::chrono::time_point<std::chrono::system_clock> get_last_tick_time_point() const;

		template <class ChronoDuration>
		callback_timer_t* set_interval(const ChronoDuration& interval)
		{
			this->interval = std::chrono::duration_cast<ChronoDuration>(interval);
			return this;
		}

	private:
		std::function<void(void)> function{};
		std::chrono::milliseconds interval{};
		std::chrono::time_point<std::chrono::system_clock> latest_tick_time_point{};
		std::thread thread{};
		bool should_repeat{};
		bool is_running{};
		bool is_elapsed{};
	};
}