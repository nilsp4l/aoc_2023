#include "utility.h"
#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>
#include <ranges>
#include <cmath>
#include <numeric>
#include <unordered_map>
#include <regex>

void reduce_to_twodigits(std::vector<int> &int_vec)
{
    for (int &cur : int_vec)
    {
        int digit{0};

        digit = cur % 10;
        int first_digit{digit};
        cur /= 10;
        while (cur)
        {
            digit = cur % 10;
            cur /= 10;
        }

        cur += first_digit + digit * 10;
    }
}

void build_integer_vec1(const std::vector<std::string> &string_vec, std::vector<int> &int_vec)
{
    for (const std::string &cur : string_vec)
    {
        int to_push{0};
        int offs{0};
        for (const char &cur_c : cur | std::views::reverse)
        {
            if (cur_c < 58 && cur_c > 47)
            {
                to_push += (cur_c - 48) * static_cast<int>(std::pow(10, offs++));
            }
        }
        int_vec.push_back(to_push);
    }

    reduce_to_twodigits(int_vec);
}

void build_integer_vec2(const std::vector<std::string> &string_vec, std::vector<int> &int_vec)
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

    for (const std::string &cur : string_vec)
    {
        std::smatch m;
        auto it_start{cur.begin()};
        std::vector<std::string> matches;
        while (std::regex_search(it_start, cur.end(), m, reg))
        {
            matches.push_back(m[0]);

            // necessary as e.g. "eighthree" should be interpreted as 83
            it_start = m[0].length() < 2 ? m.suffix().first : m.suffix().first - 1;
        }

        int to_push{0};
        int offs{0};
        for (auto cur_match : matches | std::views::reverse)
        {
            auto digit_iterator{map.find(cur_match)};
            if (digit_iterator == map.end())
            {
                throw std::runtime_error("could not find in map");
            }
            to_push += digit_iterator->second * static_cast<int>(std::pow(10, offs++));
        }
        int_vec.push_back(to_push);
    }
    reduce_to_twodigits(int_vec);
}

int execute_challenge1()
{
    std::vector<std::string> input = parse_file_line("../input/day1");

    std::vector<int> input_ints;
    build_integer_vec1(input, input_ints);

    return std::accumulate(input_ints.begin(), input_ints.end(), 0);
}

int execute_challenge2()
{
    std::vector<std::string> input = parse_file_line("../input/day1");
    std::vector<int> input_ints;

    build_integer_vec2(input, input_ints);

    return std::accumulate(input_ints.begin(), input_ints.end(), 0);
}

int main()
{
    std::cout << "First star: " << execute_challenge1() << ", Second Star: " << execute_challenge2() << std::endl;
}