#include <cstdlib>
#include <iostream>
#include <string>
#include <functional>
#include <ranges>
#include <array>
#include <algorithm>

using array_type = std::vector<std::string>;

array_type parse_pattern(const std::string& pattern);
bool process_input(const std::string& input, const std::string& pattern);
bool match_pattern(const char character, const std::string& pattern);

int main(int argc, char** argv) 
{
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::cout << "Logs from your program will appear here\n";

    if(argc != 3) 
    {
        std::cerr << "Error: expected two arguments\n";
        return EXIT_FAILURE;
    }

    std::string flag{argv[1]};
    std::string pattern{argv[2]};
    std::cout << pattern << '\n';

    if(flag != "-E") 
    {
        std::cerr << "Error: expected first argument to be '-E'\n";
        return EXIT_FAILURE;
    }

    std::string input{};
    std::cin >> std::noskipws;
    std::getline(std::cin, input);
    std::cout << input << '\n';
    try 
    {
        if(process_input(input, pattern)) 
        {
            std::cout << "Matched\n";
            return EXIT_SUCCESS;
        } 
        else 
        {
            std::cout << "Unmatched\n";
            return EXIT_FAILURE;
        }
    } 
    catch(const std::runtime_error& error) 
    {
        std::cerr << error.what() << '\n';
        return EXIT_FAILURE;
    }
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
    std::cout << std::size(patterns) << '\n';
    for(const auto ch : input)
    {
        if(currentPattern >= std::size(patterns))
        {
            break;
        }
        if(match_pattern(ch, patterns[currentPattern]))
        {
            ++currentPattern;
        }
        else
        {
            currentPattern = 0;
        }
    }
    return currentPattern >= std::size(patterns);
}

bool match_pattern(const char character, const std::string& pattern)
{
    if(pattern == "\\d")
    {
        return static_cast<bool>(std::isdigit(character));
    }
    else if(pattern == "\\w")
    {
        return static_cast<bool>(std::isalnum(character));
    }
    else if(pattern.length() == 1) 
    {
        return pattern[0] == character;
    }
    else if(auto start{pattern.find("[")}, finish{pattern.find("]")}; 
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
    else 
    {
        throw std::runtime_error("Error: unhandled pattern " + pattern);
    }
    return false;
}