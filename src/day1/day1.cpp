#include "utility.h"
#include <iostream>
#include <string>
#include <cmath>
#include <unordered_map>
#include <regex>

int do_task1(const std::vector<std::string> &string_vec)
{
    int acc{0};
    for (const std::string &cur : string_vec)
    {
        int first_dig{-1};
        int second_dig{-1};
        for (const char &cur_c : cur)
        {
            if (cur_c < 58 && cur_c > 47 && first_dig < 0)
            {
                first_dig = cur_c - 48;
            }

            if (cur_c < 58 && cur_c > 47)
            {
                second_dig = cur_c - 48;
            }
        }
        acc += second_dig + first_dig * 10;
    }

    return acc;
}

int do_task2(const std::vector<std::string> &string_vec)
{
    std::unordered_map<std::string, int> map =
        {
            {"zero", 0},
            {"one", 1},
            {"two", 2},
            {"three", 3},
            {"four", 4},
            {"five", 5},
            {"six", 6},
            {"seven", 7},
            {"eight", 8},
            {"nine", 9},
            {"0", 0},
            {"1", 1},
            {"2", 2},
            {"3", 3},
            {"4", 4},
            {"5", 5},
            {"6", 6},
            {"7", 7},
            {"8", 8},
            {"9", 9},
        };
    const std::regex reg{"(zero|one|two|three|four|five|six|seven|eight|nine|[0-9])"};

    int acc{0};
    for (const std::string &cur : string_vec)
    {
        std::smatch m;
        auto it_start{cur.begin()};
        std::string first_digit{""};
        std::string second_digit{""};
        while (std::regex_search(it_start, cur.end(), m, reg))
        {
            if (first_digit.length() == 0)
            {
                first_digit = m[0];
            }

            second_digit = m[0];

            // necessary as e.g. "eighthree" should be interpreted as 83
            it_start = m[0].length() < 2 ? m.suffix().first : m.suffix().first - 1;
        }

        auto second_digit_iterator{map.find(second_digit)};
        auto first_digit_iterator{map.find(first_digit)};
        if (first_digit_iterator == map.end() || second_digit_iterator == map.end())
        {
            throw std::runtime_error("could not find in map");
        }
        acc += second_digit_iterator->second + first_digit_iterator->second * 10;
    }
    return acc;
}

std::pair<int, int> execute_challenges()
{
    std::vector<std::string> input = parse_file_line("../input/day1");
    int task1{do_task1(input)};
    int task2{do_task2(input)};
    return std::make_pair(task1, task2);
}

int main()
{
    std::pair<int, int> results{execute_challenges()};
    std::cout << "First star: " << results.first << ", Second Star: " << results.second << std::endl;
}