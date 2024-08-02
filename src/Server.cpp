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
            return EXIT_SUCCESS;
        } 
        else 
        {
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
    const char* patt{};
    if(pattern == R"(\d)")
    {
        return std::find_if(std::begin(inputLine), std::end(inputLine), [](const auto letter)
        {
            return static_cast<bool>(std::isdigit(letter));
        }) != std::end(inputLine);
    }
    else if(pattern.length() == 1) 
    {
        
        return inputLine.find(pattern) != std::string::npos;
    }
    else 
    {
        throw std::runtime_error("Unhandled pattern " + pattern);
    }
}