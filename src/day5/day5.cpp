#include "utility.h"
#include <ranges>
#include <algorithm>
#include <iostream>

struct map
{
    long destination{-1};
    long source{-1};
    long range{-1};
};

struct source
{
    long source_num;
    bool mapped;
};



long find_mapping(long seed, const struct map &map)
{
    if (seed < map.source + map.range && seed >= map.source)
    {
        return map.destination + (seed - map.source);
    }

    return 0;
}

long find_mapping_rev(long seed, const struct map &map)
{
    if (seed < map.destination + map.range && seed >= map.destination)
    {
        return map.source + (seed - map.destination);
    }

    return 0;
}

std::vector<std::string> split(const std::string string, char delimiter)
{
    std::vector<std::string> to_return;
    for (const auto word : std::views::split(string, delimiter))
    {
        to_return.push_back(std::string{word.begin(), word.end()});
    }

    return to_return;
}

std::vector<struct source> get_seeds(const std::string &seed_line)
{
    std::vector<std::string> seed_string{split(seed_line, ' ')};

    std::vector<struct source> to_return;
    for (std::size_t i{1}; i < seed_string.size(); ++i)
    {
        struct source current
        {
            std::stol(seed_string.at(i)), false
        };
        to_return.push_back(current);
    }

    return to_return;
}

struct map get_map_line(const std::string &seed_line)
{
    std::vector<std::string> sources{split(seed_line, ' ')};

    struct map current;

    current.destination = std::stol(sources.at(0));
    current.source = std::stol(sources.at(1));
    current.range = std::stol(sources.at(2));

    return current;
}

long find_min(const std::vector<struct source> &sources)
{
    long min = __LONG_LONG_MAX__;
    for (const auto source : sources)
    {
        if (source.source_num < min)
        {
            min = source.source_num;
        }
    }
    return min;
}

long do_task1(const std::vector<std::string> &lines)
{
    std::vector<struct source> sources{get_seeds(lines.at(0))};
    bool build_map_flag{false};
    for (std::size_t i{1}; i < lines.size(); ++i)
    {
        if (lines.at(i).empty())
        {
            build_map_flag = false;
            continue;
        }

        if (!build_map_flag && (lines.at(i).find("map:") != std::string::npos))
        {
            build_map_flag = true;
            for (struct source &source : sources)
            {
                source.mapped = false;
            }
            continue;
        }

        struct map current_map_line
        {
            get_map_line(lines.at(i))
        };

        for (struct source &source : sources)
        {
            if (source.mapped)
            {
                continue;
            }
            long mapping{find_mapping(source.source_num, current_map_line)};
            if (mapping > 0)
            {
                source.source_num = mapping;
                source.mapped = true;
            }
        }
    }

    return find_min(sources);
}

bool seed_exists(std::vector<struct source> seeds, long current)
{
    for (std::size_t i{0}; i < seeds.size() - 1; i += 2)
    {
        if (current >= seeds.at(i).source_num && current < seeds.at(i).source_num + seeds.at(i + 1).source_num)
        {
            return true;
        }
    }
    return false;
}

std::vector<std::vector<struct map>> initialize_maps(std::vector<std::string> lines)
{

    std::vector<std::vector<struct map>> to_return;
    std::size_t map_count{0};
    for (std::size_t i{1}; i < lines.size(); ++i)
    {

        if (lines.at(i).empty())
        {
            continue;
        }

        if ((lines.at(i).find("map:") == std::string::npos))
        {
            continue;
        }

        std::vector<struct map> to_push;
        to_return.push_back(to_push);
        for (std::size_t j{i + 1}; j < lines.size(); ++j)
        {

            if (lines.at(j).empty())
            {
                i = j;
                break;
            }
            to_return.at(map_count).push_back(get_map_line(lines.at(j)));
        }
        ++map_count;
    }

    return to_return;
}

// brute force :(
long do_task2(std::vector<std::string> lines)
{

    long current_init{46};

    auto maps{initialize_maps(lines)};

    auto seeds(get_seeds(lines.at(0)));

    while (true)
    {
        long current_try{current_init};
        for (long i{static_cast<long>(maps.size() - 1)}; i >= 0; --i)
        {

            for (std::size_t j{0}; j < maps.at(i).size(); ++j)
            {
                long mapping{find_mapping_rev(current_try, maps.at(i).at(j))};
                if (mapping > 0)
                {
                    current_try = mapping;
                    break;
                }
            }
        }

        if (seed_exists(seeds, current_try))
        {
            return current_init;
        }
        ++current_init;
    }
}

std::pair<long, long> execute_tasks()
{
    return {do_task1(parse_file_line("../input/day5")), do_task2(parse_file_line("../input/day5"))};
}

int main()
{
    auto results{execute_tasks()};

    std::cout << "First star: " << results.first << ", Second Star: " << results.second << std::endl;
    return 0;
}