#include <iostream>
#include <regex>
#include <unordered_map>
#include <string_view>
#include <functional>

#include "utility.h"

std::unordered_map<std::string, int> get_line_map(const std::string &line, std::regex reg)
{
    std::unordered_map<std::string, int> map{
        {"Game", 0},
        {"blue", 0},
        {"red", 0},
        {"green", 0}};

    auto it_start{line.begin()};
    std::smatch m;
    std::string cur_token{""};
    std::string cur_name{""};
    std::string cur_attribute{""};
    
    while (std::regex_search(it_start, line.end(), m, reg))
    {
        cur_token = m[0];

        std::size_t whitespace_pos = cur_token.find(' ');
        if (std::isdigit(cur_token.at(0)))
        {
            cur_attribute = cur_token.substr(0, whitespace_pos);
            cur_name = cur_token.substr(whitespace_pos + 1, cur_token.length());
        }
        else
        {
            cur_name = cur_token.substr(0, whitespace_pos);
            cur_attribute = cur_token.substr(whitespace_pos + 1, cur_token.length());
        }

        auto token_entry{map.find(cur_name)};
        if (token_entry == map.end())
        {
            throw std::runtime_error{"token not in map"};
        }

        int attr{std::stoi(cur_attribute)};
        if (attr > token_entry->second)
        {
            token_entry->second = attr;
        }

        it_start = m.suffix().first;
    }
    return map;
}

int get_line_value(const std::unordered_map<std::string, int> &map, std::function<int(int, int, int, int)> func)
{
    auto red_it{map.find("red")};
    auto green_it{map.find("green")};
    auto blue_it{map.find("blue")};
    auto game_it{map.find("Game")};

    if (red_it == map.end() || green_it == map.end() || blue_it == map.end() || game_it == map.end())
    {
        throw std::runtime_error{"token not in map"};
    }
    return func(game_it->second, red_it->second, green_it->second, blue_it->second);
}

int do_task1(std::vector<std::string> lines)
{
    std::regex reg{"Game [0-9]+|[0-9]+ (blue|red|green)"};
    int acc{0};

    for (const std::string &line : lines)
    {
        std::unordered_map<std::string, int> map{get_line_map(line, reg)};
        auto red_it{map.find("red")};
        auto green_it{map.find("green")};
        auto blue_it{map.find("blue")};
        auto game_it{map.find("Game")};

        if (red_it == map.end() || green_it == map.end() || blue_it == map.end() || game_it == map.end())
        {
            throw std::runtime_error{"token not in map"};
        }

        if (red_it->second <= 12 && green_it->second <= 13 && blue_it->second <= 14)
        {
            acc += game_it->second;
        }
    }

    return acc;
}

int do_task(std::vector<std::string> lines, bool task1)
{
    std::regex reg{"Game [0-9]+|[0-9]+ (blue|red|green)"};
    int acc{0};

    for (const std::string &line : lines)
    {
        std::unordered_map<std::string, int> map{get_line_map(line, reg)};

        if (task1)
        {
            acc += get_line_value(map, [](int game, int red, int green, int blue)
                                  { if(red <= 12 && green <= 13 && blue <= 14)
                                  {
                                    return game;
                                  } 
                                    return 0; });
        }
        else
        {
            acc += get_line_value(map, [](int, int red, int green, int blue)
                                  { return red * green * blue; });
        }
    }

    return acc;
}

std::pair<int, int> execute_challenges()
{
    std::vector<std::string> input{parse_file_line("../input/day2")};
    int task1 = do_task(input, true);
    int task2 = do_task(input, false);
    return std::make_pair(task1, task2);
}

int main()
{
    std::pair<int, int> results{execute_challenges()};
    std::cout << "First star: " << results.first << ", Second Star: " << results.second << std::endl;
}
