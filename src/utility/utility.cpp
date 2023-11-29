#include "utility.h"

auto parse_file(const std::string& path) -> std::string
{
    std::ifstream input_file;

    input_file.open(path);

    if (!input_file.is_open())
    {
        return "error";
    }
    
    std::stringstream buf;

    buf << input_file.rdbuf();

    return buf.str();
}

