#include <ranges>
#include <iostream>
#include "utility.h"

std::vector<long> split_ints(std::string_view string, char delimiter)
{
    std::vector<long> to_return;
    for (const auto numb : std::views::split(string, delimiter))
    {
        to_return.push_back(std::stol(std::string(numb.begin(), numb.end())));
    }

    return to_return;
}

void caclulate_differences(std::vector<std::vector<long>> &to_calc)
{
    bool zero_flag{false};

    while (!zero_flag)
    {
        std::vector<long> cur{to_calc.at(to_calc.size() - 1)};
        std::vector<long> cur_dif;
        zero_flag = true;
        for (std::size_t i{0}; i < cur.size() - 1; ++i)
        {
            long to_push{cur.at(i + 1) - cur.at(i)};
            if (to_push)
            {
                zero_flag = false;
            }
            cur_dif.push_back(to_push);
        }
        to_calc.push_back(cur_dif);
    }
}

long do_task1(const std::vector<std::vector<long>> &cur_diffs)
{
    long cur_placeholder{0};
    long upper = static_cast<long>(cur_diffs.size() - 1);
    cur_placeholder = cur_diffs.at(upper).at(cur_diffs.at(upper).size() - 2) + cur_diffs.at(upper - 1).at(cur_diffs.at(upper - 1).size() - 1);
    for (long i{upper - 1}; i > 0; --i)
    {
        cur_placeholder += cur_diffs.at(i - 1).at(cur_diffs.at(i - 1).size() - 1);
    }

    return cur_placeholder;
}

long do_task2(const std::vector<std::vector<long>> &cur_diffs)
{
    long cur_placeholder{0};
    long upper = static_cast<long>(cur_diffs.size() - 1);
    cur_placeholder = cur_diffs.at(upper - 1).at(1) - cur_diffs.at(upper).at(0);
    for (long i{static_cast<long>(upper - 1)}; i > 0; --i)
    {
        cur_placeholder = cur_diffs.at(i - 1).at(0) - cur_placeholder;
    }

    return cur_placeholder;
}

std::pair<long, long> do_tasks(std::vector<std::string> lines)
{

    long acc1{0};
    long acc2{0};

    for (const std::string &line : lines)
    {
        std::vector<std::vector<long>> cur_diffs;
        std::vector<long> cur{split_ints(line, ' ')};
        cur_diffs.push_back(cur);
        caclulate_differences(cur_diffs);

        acc1 += do_task1(cur_diffs);
        acc2 += do_task2(cur_diffs);
    }

    return {acc1, acc2};
}

int main()
{
    const auto results{do_tasks(parse_file_line("../input/day9"))};

    std::cout << "First star: " << results.first << ", Second Star: " << results.second << std::endl;
}