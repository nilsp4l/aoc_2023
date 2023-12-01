#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include <string>

auto parse_file(const std::string& path) -> std::string;
auto parse_file_line(const std::string& path) -> std::vector<std::string>;