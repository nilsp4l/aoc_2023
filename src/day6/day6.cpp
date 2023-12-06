#include "utility.h"
#include <ranges>
#include <iostream>
#include <cmath>

std::vector<long> split(std::string_view string, char delimiter)
{

    std::vector<long> to_return;
    for (const auto word : std::views::split(string, delimiter))
    {
        if (word.empty() || !std::isdigit(word[0]))
        {
            continue;
        }
        to_return.push_back(std::stoi(std::string{word.begin(), word.end()}));
    }

    return to_return;
}

std::vector<std::pair<long, long>> get_time_distance_mapping1(const std::vector<std::string> &lines)
{
    int pos{0};
    std::vector<std::pair<long, long>> to_return;
    std::vector<long> first;
    std::vector<long> second;

    for (std::string_view line : lines)
    {

        for (std::size_t i{0}; i < line.size(); ++i)
        {
            if (std::isdigit(line.at(i)) && !pos)
            {
                first = split(line.substr(i, line.size()), ' ');
                break;
            }
            else if (std::isdigit(line.at(i)))
            {
                second = split(line.substr(i, line.size()), ' ');
                break;
            }
        }
        ++pos;
    }

    for (auto first_it{first.begin()}, second_it{second.begin()}; first_it != first.end() && second_it != second.end(); ++first_it, ++second_it)
    {
        to_return.push_back({*first_it, *second_it});
    }
    return to_return;
}

// maybe the stupidest way to do this but didnt want to rewrite everything after part 1
long build_long(const std::vector<long> &vec)
{
    long to_return{0};
    long offset{1};
    for (long i{static_cast<long>(vec.size() - 1)}; i >= 0; --i)
    {
        long cur{vec.at(i)};
        to_return += offset * cur;

        while (cur > 0)
        {
            cur /= 10;
            offset *= 10;
        }
    }
    return to_return;
}

std::vector<std::pair<long, long>> get_time_distance_mapping2(const std::vector<std::string> &lines)
{
    int pos{0};
    std::vector<std::pair<long, long>> to_return;
    std::vector<long> first;
    std::vector<long> second;

    for (std::string_view line : lines)
    {

        for (std::size_t i{0}; i < line.size(); ++i)
        {
            if (std::isdigit(line.at(i)) && !pos)
            {
                first = split(line.substr(i, line.size()), ' ');
                break;
            }
            else if (std::isdigit(line.at(i)))
            {
                second = split(line.substr(i, line.size()), ' ');
                break;
            }
        }
        ++pos;
    }

    return {std::make_pair(build_long(first), build_long(second))};
}

long quadratic_formula(long time, long distance)
{
    double time_d{static_cast<double>(time)};
    double distance_d{static_cast<double>(distance)};
    double disc{time_d * time_d - 4. * distance_d};
    if (disc < 0)
    {
        return 0;
    }
    double x1{((-time_d - std::sqrt(disc)) / -2)};
    double x2{((-time_d + std::sqrt(disc)) / -2)};

    long x1_ret{static_cast<long>(std::ceil(x1 - 1))};
    
    long x2_ret{static_cast<long>(std::floor(x2 + 1))};
    
    return x1_ret - x2_ret + 1;
}

long do_tasks(const std::vector<std::pair<long, long>> &td_map)
{

    long acc{1};

    for (auto [time, distance] : td_map)
    {
        acc *= quadratic_formula(time, distance);
    }

    return acc;
}

long execute_task(const std::vector<std::string> lines, bool task1)
{
    std::vector<std::pair<long, long>> td_map{get_time_distance_mapping1(lines)};
    if (task1)
    {
        return do_tasks(get_time_distance_mapping1(lines));
    }
    else
    {
        return do_tasks(get_time_distance_mapping2(lines));
    }
}

int main()
{
    long first{execute_task(parse_file_line("../input/day6"), true)};
    long second{execute_task(parse_file_line("../input/day6"), false)};

    std::cout << "First star: " << first << ", Second Star: " << second << std::endl;
    return 0;
}