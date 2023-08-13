#pragma once
#include <cstdint>
#include <random>


namespace rutils
{
	class random_generator_t
	{
	public:
		static int32_t get_rand_int(int32_t min, int32_t max);
		static float get_rand_float(float min, float max);

	private:
		static std::random_device rand_device;
		static std::mt19937 rand_generator;
	};
}


