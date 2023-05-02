#include "callback_timer.h"

rutils::callback_timer_t::callback_timer_t(std::function<void(void)>&& func, const int32_t interval, const bool should_repeat, const bool auto_start)
	: callback_timer_t(std::move(func), std::chrono::milliseconds(interval), should_repeat, auto_start)
{
}

rutils::callback_timer_t::callback_timer_t(std::function<void(void)>&& func, const std::chrono::milliseconds& interval, const bool should_repeat, const bool auto_start)
	: function{ std::move(func)}
	, interval{ interval }
	, latest_tick_time_point(std::chrono::system_clock::now())
	, should_repeat{ should_repeat }
	, is_running{ false }
	, is_elapsed{ false }
{
	if (auto_start)
		start();
}

rutils::callback_timer_t::~callback_timer_t()
{
	stop();
}

void rutils::callback_timer_t::start()
{
	is_running = true;
	is_elapsed = false;

	latest_tick_time_point = std::chrono::system_clock::now();

	thread = std::thread([&]()
	{
		do
		{
			std::this_thread::sleep_for(interval);
			function();
			latest_tick_time_point = std::chrono::system_clock::now();
		} while (should_repeat);

		is_elapsed = true;
	});

	thread.detach();
}

void rutils::callback_timer_t::stop()
{
	is_running = false;
	thread.~thread();
}

void rutils::callback_timer_t::restart()
{
	stop();
	start();
}

bool rutils::callback_timer_t::get_is_elapsed() const
{
	return is_elapsed;
}

bool rutils::callback_timer_t::is_runnning() const
{
	return is_running;
}

bool rutils::callback_timer_t::is_repeating() const
{
	return should_repeat;
}

rutils::callback_timer_t* rutils::callback_timer_t::set_function(std::function<void(void)>&& func)
{
	function = func;
	return this;
}

rutils::callback_timer_t* rutils::callback_timer_t::set_interval(const uint32_t interval)
{
	set_interval(std::chrono::milliseconds(interval));
	return this;
}

rutils::callback_timer_t* rutils::callback_timer_t::set_repeating(const bool should_repeat)
{
	this->should_repeat = should_repeat;
	return this;
}

std::chrono::time_point<std::chrono::system_clock> rutils::callback_timer_t::get_next_tick_time_point() const
{
	return get_last_tick_time_point() + interval;
}

std::chrono::time_point<std::chrono::system_clock> rutils::callback_timer_t::get_last_tick_time_point() const
{
	return latest_tick_time_point;
}
