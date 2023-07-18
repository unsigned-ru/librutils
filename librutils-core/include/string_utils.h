#pragma once
#include <string>
#include <vector>
#include <sstream>

namespace rutils
{
    std::string indent(const std::string& src, size_t level, char delimiter = ' ');
    std::string prefix_fill(const std::string& src, const int desired_length, const char fill_char = '0');
    std::string suffix_fill(const std::string& src, const int desired_length, const char fill_char = '0');
	std::string to_lower(const std::string& src);
    std::string shrink_to_fit(const std::string& src, const size_t max_length, const std::string& truncation = "...");
    std::string replace_all(std::string str, const std::string& from, const std::string& to);
    std::string load_file_data(const std::string& path);
    std::string remove_from_back_untill(const std::string& string, const std::string& untill_chars);
    std::string join(const std::vector<std::string>& strings, const std::string& delim);
    std::vector<std::string> split_string_by_delimiters(const std::string& string, const std::string& delimiters);

    bool starts_with(const std::string& s, const std::string& starts_with);

    void left_trim(std::string& s);
    void right_trim(std::string& s);
    void trim(std::string& s);
    void remove_excessive_whitespace(std::string& s);

    std::string left_trim_copy(std::string s);
    std::string right_trim_copy(std::string s);
    std::string trim_copy(std::string s);
    std::string remove_excessive_whitespace_copy(std::string s);

    template <typename integral_type_t>
	requires std::is_integral_v<integral_type_t>
    integral_type_t hex_to_int(const std::string& hex_code)
    {
        //TODO: check for failure, bad stream etc

        integral_type_t rv;
        std::stringstream ss;
        ss << std::hex << hex_code;
        ss >> rv;

        return rv;
    }
}
