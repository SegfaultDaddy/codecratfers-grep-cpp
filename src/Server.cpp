#include <cstdlib>
#include <string_view>
#include <iostream>
#include <string>
#include <print>

bool match_pattern(const std::string& input_line, const std::string& pattern);

int main(int argc, char** argv) 
{
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::println(std::cout, "Logs from your program will appear here");

    if(argc != 3) {
        std::println(std::cerr, "Expected two arguments");
        return EXIT_FAILURE;
    }

    std::string flag{argv[1]};
    std::string pattern{argv[2]};

    if(flag != "-E") {
        std::println("Expected first argument to be '-E'");
        return EXIT_FAILURE;
    }

    std::string input_line{};
    std::getline(std::cin, input_line);
    
    try 
    {
        if(match_pattern(input_line, pattern)) 
        {
            return EXIT_SUCCESS;
        } 
        else 
        {
            return EXIT_FAILURE;
        }
    } 
    catch(const std::runtime_error& e) 
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_FAILURE;
}

bool match_pattern(const std::string& input_line, const std::string& pattern)
{
    if(pattern.length() == 1) 
    {
        return input_line.find(pattern) != std::string::npos;
    }
    else 
    {
        throw std::runtime_error("Unhandled pattern " + pattern);
    }
}