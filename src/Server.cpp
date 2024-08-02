#include <cstdlib>
#include <iostream>
#include <string>
#include <algorithm>

bool match_pattern(const std::string& inputLine, const std::string& pattern);

int main(int argc, char** argv) 
{
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::cout << "Logs from your program will appear here\n";

    if(argc != 3) 
    {
        std::cerr << "Expected two arguments\n";
        return EXIT_FAILURE;
    }

    std::string flag{argv[1]};
    std::string pattern{argv[2]};
    std::cout << pattern << '\n';
    if(flag != "-E") 
    {
        std::cerr << "Expected first argument to be '-E'\n";
        return EXIT_FAILURE;
    }

    std::string inputLine{};
    std::getline(std::cin, inputLine);
    
    try 
    {
        if(match_pattern(inputLine, pattern)) 
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

bool match_pattern(const std::string& inputLine, const std::string& pattern)
{
    if(pattern == R"(\d)")
    {
        return std::find_if(std::begin(inputLine), std::end(inputLine), [](const auto character)
        {
            return static_cast<bool>(std::isdigit(character));
        }) != std::end(inputLine);
    }
    else if(pattern == R"(\w)")
    {
        return std::find_if(std::begin(inputLine), std::end(inputLine), [](const auto character)
        {
            return static_cast<bool>(std::isalnum(character));
        }) != std::end(inputLine);

    }
    else if(pattern.length() == 1) 
    {
        return inputLine.find(pattern) != std::string::npos;
    }
    else if(auto start{pattern.find("[")}, finish{pattern.find("]")}; 
            (start != std::string::npos && finish != std::string::npos) && (start < finish))
    {
        auto characterGroup{pattern.substr(start + 1, finish - start - 1)};
        for(const auto& character : characterGroup)
        {
            if(inputLine.find(character) != std::string::npos)
            {
                return true;
            }
        }
    }
    else 
    {
        throw std::runtime_error("Unhandled pattern " + pattern);
    }
    return false;
}