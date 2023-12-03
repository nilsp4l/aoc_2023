#include "utility.h"
#include <regex>
#include <iostream>


// ugly but works i guess

int do_reg_search(size_t left_border, size_t sanitized_length, const std::string &to_check, int &adj_numb_count)
{
    int acc{1};
    auto it_start = to_check.cbegin();
    std::smatch m;
    
    std::regex reg{"[0-9]+"};
    std::size_t pos_off{0};
    while (std::regex_search(it_start, to_check.cend(), m, reg))
    {
        if(pos_off + m.position() > left_border + sanitized_length - 1)
        {
            break;
        }

        if(((pos_off + m.position() >= left_border) && (pos_off + m.position() <= left_border + sanitized_length - 1)) 
        || ((pos_off + m.position() + m[0].length() - 1 >= left_border) && (pos_off + m.position() + m[0].length() <= left_border + sanitized_length - 1)))
        {
            ++adj_numb_count;
            acc *= std::stoi(m[0]);
        }
        pos_off = std::distance(to_check.begin(), m.suffix().first);
        it_start = m.suffix().first;
    }
    return acc;
}

int find_colliding_numbers(std::size_t pos, const std::string &before, const std::string &current, const std::string &after, std::size_t line_size)
{
    int acc{1};

    std::size_t length{1};
    std::size_t sanitized_length{length};
    std::size_t left_border{pos};

    if (pos > 0)
    {
        --left_border;
        ++sanitized_length;
    }
    if (pos + length + 1 < line_size)
    {
        ++sanitized_length;
    }
    
    
    
    int adj_numb_count = 0;
    acc *= do_reg_search(left_border, sanitized_length, before, adj_numb_count);
    acc *= do_reg_search(left_border, sanitized_length, current, adj_numb_count);
    acc *= do_reg_search(left_border, sanitized_length, after, adj_numb_count);

    if(adj_numb_count == 2)
    {
        return acc;
    }

    return 0;
}

int do_task2(const std::vector<std::string> &input)
{
    int offset{-1};
    int acc{0};
    for (std::size_t i{0}; i < input.size(); ++i)
    {
        std::size_t pos{0};

        std::string before{""};
        std::string current{input.at(i)};
        std::string after{""};

        if (i > 0)
        {
            before = input.at(i - 1);
        }

        if (i < input.size() - 1)
        {
            after = input.at(i + 1);
        }

        while ((pos = input.at(i).find_first_of('*', offset + 1)) != std::string::npos)
        {
            offset = static_cast<int>(pos);
            acc += find_colliding_numbers(pos, before, current, after, current.size());
        }
        offset = -1;
    }
    return acc;
}

bool check_for_symbols(std::size_t pos, std::size_t length, const std::string_view &before, const std::string_view &current, const std::string_view &after, std::size_t line_size)
{
    std::size_t sanitized_length{length};
    std::size_t left_border{pos};

    if (pos > 0)
    {
        --left_border;
        ++sanitized_length;
    }
    if (pos + length + 1 < line_size)
    {
        ++sanitized_length;
    }

    std::string_view bef_sub{""};
    std::string_view after_sub{""};
    if (!before.empty())
    {
        bef_sub = before.substr(left_border, sanitized_length);
    }

    if (!after.empty())
    {
        after_sub = after.substr(left_border, sanitized_length);
    }
    std::string_view cur_sub{current.substr(left_border, sanitized_length)};

    std::string char_set{"0123456789."};

    return bef_sub.find_first_not_of(char_set) != std::string::npos || cur_sub.find_first_not_of(char_set) != std::string::npos || after_sub.find_first_not_of(char_set) != std::string::npos;
}

int do_task1(const std::vector<std::string> &input)
{
    std::regex reg{"[0-9]+"};

    int acc{0};

    std::string_view before;
    std::string_view after;
    std::string_view current;
    for (std::size_t i{0}; i < input.size(); ++i)
    {
        if (i > 0)
        {
            before = input.at(i - 1);
        }

        current = input.at(i);

        if (i < input.size() - 1)
        {
            after = input.at(i + 1);
        }

        auto it_start = input.at(i).begin();
        std::smatch m;

        std::size_t pos_offset{0};
        while (std::regex_search(it_start, input.at(i).end(), m, reg))
        {
            if (check_for_symbols(pos_offset + m.position(), m[0].length(), before, current, after, current.length()))
            {
                acc += std::stoi(m[0]);
            }

            pos_offset = std::distance(input.at(i).begin(), m.suffix().first);

            it_start = m.suffix().first;
        }
    }
    return acc;
}

std::pair<int, int> execute_tasks()
{
    int task1{do_task1(parse_file_line("../input/day3"))};
    int task2{do_task2(parse_file_line("../input/day3"))};
    return {task1, task2};
}

int main()
{
    std::pair<int, int> results{execute_tasks()};
    std::cout << "First star: " << results.first << ", Second Star: " << results.second << std::endl;
}
