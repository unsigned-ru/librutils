#include "random.h"

std::random_device rutils::random_generator_t::rand_device{};
std::mt19937 rutils::random_generator_t::rand_generator{rand_device()};

int32_t rutils::random_generator_t::get_rand_int(int32_t min, int32_t max)
{
	std::uniform_int_distribution dist(min, max);

	return dist(rand_generator);
}

float rutils::random_generator_t::get_rand_float(float min, float max)
{
	std::uniform_real_distribution dist(min, max);

	return dist(rand_generator);
}