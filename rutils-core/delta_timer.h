#ifndef DELTA_TIMER_H
#define DELTA_TIMER_H

namespace rutils
{
	class delta_timer_t final
	{
	public:
		delta_timer_t(const float interval, const bool repeat = false, const bool startEnabled = true);
		~delta_timer_t() = default;

		bool tick(const float delta_time);

		bool is_elapsed() const;
		float get_elapsed() const;
		bool get_repeating() const;

		void start();
		void stop();

		void set_enabled(const bool enabled);
		void set_repeating(const bool repeat);
		void set_interval(const float interval);
		void reset(const bool stop = false);

	private:
		bool is_enabled;
		bool repeat;
		float interval;
		float elapsed_time;

	};
}



#endif // Include Guard: DELTA_TIMER_H
