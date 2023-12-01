#include "utility.h"


auto parse_file_line(const std::string& path) -> std::vector<std::string>
{
    std::ifstream input_file;

    input_file.open(path);

    if (!input_file.is_open())
    {
        throw std::runtime_error("file could not be opened");
    }
    
    
    std::string line;
    std::vector<std::string> to_return;

    while(std::getline(input_file, line))
    {
        to_return.push_back(line);
    }

    return to_return;
}



auto parse_file(const std::string& path) -> std::string
{
    std::ifstream input_file;

    input_file.open(path);

    if (!input_file.is_open())
    {
        throw std::runtime_error("file could not be opened");
    }
    
    std::stringstream buf;

    buf << input_file.rdbuf();

    return buf.str();
}

