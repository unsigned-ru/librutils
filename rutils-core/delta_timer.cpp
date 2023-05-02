#include "delta_timer.h"
#include <cmath>

rutils::delta_timer_t::delta_timer_t(const float interval, const bool repeat, const bool start_enabled)
	: is_enabled{ start_enabled }
	, repeat{repeat}
	, interval{interval}
	, elapsed_time{0.f}
{
}

bool rutils::delta_timer_t::tick(const float delta_time)
{
	if (!is_enabled || (!repeat && is_elapsed()))
		return false;

	elapsed_time += delta_time;

	if (is_elapsed())
	{
		//we elapsed
		//check if we need to repeat
		if (repeat)
		{
			//set elapsedTime to remainder
			elapsed_time = fmod(elapsed_time, interval);
		}

		return true;
	}

	return false;
}

bool rutils::delta_timer_t::is_elapsed() const
{
	return elapsed_time >= interval;
}

float rutils::delta_timer_t::get_elapsed() const
{
	return elapsed_time;
}

bool rutils::delta_timer_t::get_repeating() const
{
	return repeat;
}

void rutils::delta_timer_t::start()
{
	set_enabled(true);
}

void rutils::delta_timer_t::stop()
{
	set_enabled(false);
}

void rutils::delta_timer_t::set_enabled(const bool enabled)
{
	is_enabled = enabled;
}

void rutils::delta_timer_t::set_repeating(const bool repeat)
{
	this->repeat = repeat;
}

void rutils::delta_timer_t::set_interval(const float interval)
{
	this->interval = interval;
}

void rutils::delta_timer_t::reset(const bool stop)
{
	elapsed_time = 0.f;
	
	if (stop)
		this->stop();
}
