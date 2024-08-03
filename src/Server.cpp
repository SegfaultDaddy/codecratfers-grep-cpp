#include <cstdlib>
#include <iostream>
#include <string>
#include <functional>
#include <ranges>
#include <optional>
#include <array>
#include <algorithm>

using array_type = std::vector<std::string>;

array_type parse_pattern(const std::string& pattern);
bool process_input(const std::string& input, const std::string& pattern);
std::size_t match_one_or_more(const std::size_t index, const std::string& input, const std::string& pattern);
std::optional<std::size_t> match_zero_or_one(const std::size_t index, const std::string& input, const std::string& pattern);
bool match_start_anchor(const std::size_t index, const std::string& input, const std::string& pattern);
bool match_end_anchor(const std::size_t index, const std::string& input, const std::string& pattern);
bool match_group(const char ch, const std::string& pattern);
bool match_class(const char ch, const std::string& pattern);

int main(int argc, char** argv) 
{
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if(argc != 3) 
    {
        std::cerr << "Error: expected two arguments\n";
        return EXIT_FAILURE;
    }

    std::string flag{argv[1]};
    std::string pattern{argv[2]};

    if(flag != "-E") 
    {
        std::cerr << "Error: expected first argument to be '-E'\n";
        return EXIT_FAILURE;
    }

    std::string input{};
    std::getline(std::cin, input);

    if(process_input(input, pattern)) 
    {
        return EXIT_SUCCESS;
    } 
    std::cout << "Failure\n";
    return EXIT_FAILURE;
}

array_type parse_pattern(const std::string& pattern)
{
    array_type patterns{};
    const std::size_t size{std::size(pattern)};
    for(std::size_t i{0}, j{size}; i < size; ++i)
    {   
        if(j >= size)
        {
            if(pattern[i] == '[')
            {
                j = i;
            }
            else if(pattern[i] == '\\')
            {
                patterns.emplace_back(pattern.substr(i, 2));
                ++i;
            }
            else if(pattern[i] == '+')
            {
                patterns.back() += '+';
            }
            else if(pattern[i] == '?')
            {
                patterns.back() += '?';
            }
            else
            {
                patterns.emplace_back(std::string{pattern[i]});
            }
        }
        else
        {
            if(pattern[i] == ']')
            {
                patterns.emplace_back(pattern.substr(j, i - j + 1));
                j = size;
            }
        }
    }
    return patterns;
}

bool process_input(const std::string& input, const std::string& pattern)
{
    array_type patterns{parse_pattern(pattern)};
    std::size_t currentPattern{0};
    for(std::size_t i{0}; i < std::size(input) && currentPattern < std::size(patterns); )
    {
        if(match_start_anchor(i, input, patterns[currentPattern]))
        {
            ++currentPattern;
            continue;
        }
        else if(match_group(input[i], patterns[currentPattern]))
        {
            ++currentPattern;
        }
        else if(std::size_t result{match_one_or_more(i, input, patterns[currentPattern])};
                result > i)
        {
            i = result;
            ++currentPattern;
            continue;
        }
        else if(auto result{match_zero_or_one(i, input, patterns[currentPattern])};
                result.has_value())
        {
            i = result.value();
            ++currentPattern;
            continue;
        }
        else if(match_class(input[i], patterns[currentPattern]))
        {   
            ++currentPattern;
        }
        else
        {
            currentPattern = 0;   
        }
        if(currentPattern < std::size(patterns) && match_end_anchor(i, input, patterns[currentPattern]))
        {
            ++currentPattern;
        }
        ++i;
    }
    if(std::size(patterns) - currentPattern == 1 && patterns[currentPattern].find("?") != std::string::npos)
    {
        ++currentPattern;
    }
    return currentPattern >= std::size(patterns);
}

std::size_t match_one_or_more(const std::size_t index, const std::string& input, const std::string& pattern)
{
    if(auto found{pattern.find("+")}; 
       found != std::string::npos)
    {
        std::string subPat{pattern.substr(0, found)};
        std::size_t i{index};
        while(i < std::size(input) 
              && ((match_group(input[i], subPat) || match_class(input[i], subPat))))
        {
            ++i;
        }
        return i;
    }
    return index;
}


std::optional<std::size_t> match_zero_or_one(const std::size_t index, const std::string& input, const std::string& pattern)
{
    if(auto found{pattern.find("?")}; 
       found != std::string::npos)
    {
        std::string subPat{pattern.substr(0, found)};
        bool matched{match_group(input[index], subPat) || match_class(input[index], subPat)};
        return index + matched;
    }
    return std::nullopt;
}

bool match_start_anchor(const std::size_t index, const std::string& input, const std::string& pattern)
{
    if(pattern == "^")
    {
        if(index == 0)
        {
            return true;
        }
        else
        {
            return input[index - 1] == '\n';
        }
    }
    return false;
}

bool match_end_anchor(const std::size_t index, const std::string& input, const std::string& pattern)
{
    if(pattern == "$")
    {
        if(std::size(input) - index <= 1)
        {
            return true;
        }
        else
        {
            return input[index + 1] == '\n';
        }
    }
    return false;
}

bool match_group(const char character, const std::string& pattern)
{
    if(auto start{pattern.find("[")}, finish{pattern.find("]")}; 
      (start != std::string::npos && finish != std::string::npos) && (start < finish))
    {
        bool matchCondition{true};
        if(pattern[start + 1] == '^')
        {
            matchCondition = false;
            start += 1;
        }
        for(const auto ch : std::ranges::subrange(std::cbegin(pattern) + start + 1, std::cbegin(pattern) + finish))
        {
            if(ch == character)
            {
                return matchCondition;
            }
        }
        return !matchCondition;
    }
    return false;
}

bool match_class(const char ch, const std::string& pattern)
{
    if(pattern == ".")
    {
        return true;
    }
    else if(pattern == "\\d")
    {
        return static_cast<bool>(std::isdigit(ch));
    }
    else if(pattern == "\\w")
    {
        return static_cast<bool>(std::isalnum(ch));
    }
    else if(std::size(pattern) == 1) 
    {
        return pattern[0] == ch;
    }
    return false;
}