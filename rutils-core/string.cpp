#include "string.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iterator>

std::string rutils::indent(const std::string& src, const size_t level, const char delimiter)
{
	return std::string(level * 4, delimiter) + src;
}

std::string rutils::prefix_fill(const std::string& src, const int desired_length, const char fill_char)
{
	return std::string(desired_length - src.length(), fill_char) + src;
}

std::string rutils::suffix_fill(const std::string& src, const int desired_length, const char fill_char)
{
	return src + std::string(desired_length - src.length(), fill_char);
}

std::string rutils::to_lower(const std::string& src)
{
	std::string ret{};
	ret.reserve(src.length());

	std::transform(src.begin(), src.end(), ret.begin(),
		[](char c) { return std::tolower(c); });

	return ret;
}

std::string rutils::shrink_to_fit(const std::string& src, const size_t max_length, const std::string& truncation)
{
	if (src.length() <= max_length)
		return src;

	return src.substr(0, max_length - truncation.length()) + truncation;
}

std::string rutils::replace_all(std::string str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find_first_of(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, 1, to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}



std::string rutils::load_file_data(const std::string& path)
{
	//check if the file exists
	if (!std::filesystem::exists(path))
		throw std::runtime_error("File not found.");

	std::ifstream fin(path, std::ios::in | std::ios::binary);
	std::ostringstream oss;
	oss << fin.rdbuf();

	return oss.str();
}

std::string rutils::remove_from_back_untill(const std::string& string, const std::string& untill_chars)
{
	const size_t pos = string.find_first_of(untill_chars);

	//no extention was found
	if (pos == std::string::npos)
	{
		return string;
	}

	return string.substr(0, pos);
}

std::string rutils::join(const std::vector<std::string>& strings, const std::string& delim)
{
	std::stringstream ss;
	std::ranges::copy //note will add a trailing delimiter, remove at end
	(
		strings,
	    std::ostream_iterator<std::string>(ss, delim.c_str())
	);

	const std::string joinedString = ss.str();

	//remove trailing delimiter
	return joinedString.substr(0, joinedString.length() - delim.length() );
}

std::vector<std::string> rutils::split_string_by_delimiters(const std::string& string, const std::string& delimiters)
{
	std::vector<std::string> arguments{};
	size_t start_pos{ 0 };
	size_t end_pos{ 0 };

	while (true)
	{
		//startPos = find first non whitespace character in line with offset of old end position
		if ((start_pos = string.find_first_not_of(delimiters, end_pos)) == std::string::npos)
			//we couldn't get any start position anymore.
			break;

		//endPos = find first whitespace character in line after startPos.
		if ((end_pos = string.find_first_of(delimiters, start_pos)) == std::string::npos)
			//no whitespace found after start, the end of the argument will be the end of the line.
			end_pos = string.size();

		//create argument
		std::string argument{ string.substr(start_pos, end_pos - start_pos) };
		arguments.push_back(argument);
	}

	return arguments;
}

bool rutils::starts_with(const std::string& s, const std::string& starts_with)
{
	return s.rfind(starts_with, 0) != std::string::npos;
}

void rutils::left_trim(std::string& s)
{
	s.erase
	(
		s.begin(),
		std::ranges::find_if
		(
			s,
			[](const unsigned char ch)
			{
				return !std::isspace(ch);
			}
		)
	);
}

void rutils::right_trim(std::string& s)
{
	s.erase
	(
		std::find_if
		(
			s.rbegin(),
			s.rend(),
			[](const unsigned char ch)
			{
				return !std::isspace(ch);
			}
		).base(),
	s.end()
	);
}

void rutils::trim(std::string& s)
{
	left_trim(s);
	right_trim(s);
}

std::string rutils::left_trim_copy(std::string s)
{
	left_trim(s);
	return s;
}


std::string rutils::right_trim_copy(std::string s)
{
	right_trim(s);
	return s;
}

std::string rutils::trim_copy(std::string s)
{
	trim(s);
	return s;
}

void rutils::remove_excessive_whitespace(std::string& s)
{
	trim(s);

	s = remove_excessive_whitespace_copy(s);
}


std::string rutils::remove_excessive_whitespace_copy(std::string s)
{
	trim(s);

	std::string rv;
	std::unique_copy(s.begin(), s.end(), std::back_insert_iterator(rv), [](char a, char b) { return isspace(a) && isspace(b); });
	return rv;
}