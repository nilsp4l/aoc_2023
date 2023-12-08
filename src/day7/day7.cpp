#include "utility.h"
#include <ranges>
#include <unordered_map>
#include <algorithm>
#include <iostream>

struct hand_val
{
    std::string_view hand;
    int bid;
    long score;
};

struct hand_val split(std::string_view string, char delimiter)
{
    struct hand_val to_return;
    bool bid{false};
    for (const auto word : std::views::split(string, delimiter))
    {
        if (!bid)
        {
            to_return.hand = {word.begin(), word.end()};
            bid = true;
        }
        else
        {
            to_return.bid = std::stoi(std::string{word.begin(), word.end()});
        }
    }

    return to_return;
}

// this is quite ugly for part two but i dont have time to think of a fancier approach
void calculate_score(struct hand_val &hand, const std::unordered_map<char, int> &card_map, bool second)
{
    std::array<int, 13> card_count{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (const auto card : hand.hand)
    {
        auto index_it{card_map.find(card)};

        if (index_it == card_map.end())
        {
            throw std::runtime_error("Invalid card");
        }

        ++card_count.at(index_it->second);
    }
    int max{0};
    bool two_pair_flag{false};
    bool full_house_flag{false};
    bool max_is_joker{false};
    for (int i{0}; i < static_cast<int>(card_count.size()); ++i)
    {
        int cur{card_count.at(i)};

        if (cur == max && cur == 2)
        {
            two_pair_flag = true;
        }

        if ((max == 2 && cur == 3) || (max == 3 && cur == 2))
        {
            full_house_flag = true;
            break;
        }

        if (cur > max && i == 0)
        {
            max = cur;
            max_is_joker = true;
        }
        else if(cur > max)
        {
            max = cur;
            max_is_joker = false;
        }
        else if(cur == max && max_is_joker)
        {
            max_is_joker = false;
        }
    }

    if(second && card_count.at(0) > 0)
    {
        if(card_count.at(0) == 1 && two_pair_flag)
        {
            full_house_flag = true;
            two_pair_flag = false;
        }
        else if(full_house_flag)
        {
            max = 5;
            full_house_flag = false;
        }
        else if(card_count.at(0) == 2 && two_pair_flag)
        {
            two_pair_flag = false;
            max += card_count.at(0);
        }
        else if(!max_is_joker)
        {
            max += card_count.at(0);
        }
        else
        {
            max += *std::max_element(card_count.begin() + 1, card_count.end());
        }
    }

    if (two_pair_flag)
    {
        hand.score = 1 + (1 << 4);
    }
    else if (full_house_flag)
    {
        hand.score = (1 << 4) + (1 << (4 * 2));
    }
    else
    {
        hand.score = 1 << (4 * (max - 1));
    }
}

std::vector<struct hand_val> get_hand_vals(const std::vector<std::string> &lines, const std::unordered_map<char, int> card_map, bool second)
{
    std::vector<struct hand_val> to_return;

    for (const std::string_view line : lines)
    {
        struct hand_val cur
        {
            split(line, ' ')
        };
        calculate_score(cur, card_map, second);
        to_return.push_back(cur);
    }

    return to_return;
}

long do_tasks(const std::vector<std::string> &lines, const std::unordered_map<char, int> &card_map, bool second)
{

    auto hand_vals{get_hand_vals(lines, card_map, second)};

    std::sort(hand_vals.begin(), hand_vals.end(), [&card_map](struct hand_val hand1, struct hand_val hand2)
              {
        
        if(hand1.score == hand2.score && hand1.hand.length() != hand2.hand.length())
        {
            throw std::runtime_error("invalid hand size");
        }
        
        if(hand1.score == hand2.score)
        {
            for(auto hand1_it{hand1.hand.begin()}, hand2_it{hand2.hand.begin()}; hand1_it != hand1.hand.end() && hand2_it != hand2.hand.end(); ++hand1_it, ++hand2_it)
            {
                if(*hand1_it == *hand2_it)
                {
                    continue;
                }
                return card_map.find(*hand1_it)->second > card_map.find(*hand2_it)->second;
            }
        }
        
        return hand1.score > hand2.score; });

    long acc{0};

    for (long i{static_cast<long>(hand_vals.size() - 1)}, k{1}; i >= 0; --i, ++k)
    {
        acc += (hand_vals.at(i).bid * k);
    }

    return acc;
}

std::pair<long, long> execute_tasks()
{
    std::unordered_map<char, int> card_map1{
        {'2', 0},
        {'3', 1},
        {'4', 2},
        {'5', 3},
        {'6', 4},
        {'7', 5},
        {'8', 6},
        {'9', 7},
        {'T', 8},
        {'J', 9},
        {'Q', 10},
        {'K', 11},
        {'A', 12}};

    std::unordered_map<char, int> card_map2{
        {'J', 0},
        {'2', 1},
        {'3', 2},
        {'4', 3},
        {'5', 4},
        {'6', 5},
        {'7', 6},
        {'8', 7},
        {'9', 8},
        {'T', 9},
        {'Q', 10},
        {'K', 11},
        {'A', 12}};
    long first{do_tasks(parse_file_line("../input/day7"), card_map1, false)};
    long second{do_tasks(parse_file_line("../input/day7"), card_map2, true)};
    return {first, second};
}

int main()
{
    auto results{execute_tasks()};

    std::cout << "First star: " << results.first << ", Second Star: " << results.second << std::endl;
}