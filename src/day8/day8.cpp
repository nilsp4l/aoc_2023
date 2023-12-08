#include "utility.h"
#include <memory>
#include <ranges>
#include <iostream>
#include <algorithm>
#include <numeric>

struct node
{
    std::string val;
    std::shared_ptr<node> left;
    std::shared_ptr<node> right;
};

std::string_view find_line(const std::vector<std::string> &lines, std::string_view to_look_for)
{
    for (const std::string_view line : lines)
    {
        if (line.substr(0, 3) == to_look_for)
        {
            return line;
        }
    }

    return "";
}

std::pair<std::string_view, std::string_view> get_children(std::string_view line)
{
    return std::make_pair(line.substr(7, 3), line.substr(12, 3));
}

std::shared_ptr<node> build_tree(const std::vector<std::string> &lines, std::string_view cur_val, std::vector<std::shared_ptr<node>> &stack)
{

    std::shared_ptr<struct node> root = std::make_shared<node>();

    if (auto existing{std::find_if(stack.begin(), stack.end(), [cur_val](std::shared_ptr<node> node)
                                   { return node->val == cur_val; })};
        existing != stack.end())
    {
        return *existing;
    }

    stack.push_back(root);

    std::string_view line{find_line(lines, cur_val)};

    std::pair<std::string_view, std::string_view> children = get_children(line);

    if (children.first == cur_val && children.second == cur_val)
    {

        root->val = cur_val;
        root->left = root;
        root->right = root;
    }
    else if (children.first == cur_val)
    {

        root->val = cur_val;
        root->right = root;
        root->left = build_tree(lines, children.second, stack);
    }
    else if (children.first == cur_val)
    {
        root->val = cur_val;
        root->left = build_tree(lines, children.first, stack);
        root->right = root;
    }

    else
    {
        root->val = cur_val;
        root->left = build_tree(lines, children.first, stack);
        root->right = build_tree(lines, children.second, stack);
    }

    return root;
}

long do_task1(std::shared_ptr<node> root, std::string_view instructions)
{
    std::shared_ptr cur_node{root};
    long acc{0};

    while (cur_node->val != "ZZZ")
    {
        for (const auto instruction : instructions)
        {

            switch (instruction)
            {
            case 'L':
                cur_node = cur_node->left;
                break;
            case 'R':
                cur_node = cur_node->right;
                break;
            default:
                break;
            }
            ++acc;
        }
        if (cur_node->val == "ZZZ")
        {
            return acc;
        }
    }
    return 0;
}

long compute_lcm(const std::vector<long> &nums)
{
    long acc{1};
    for(long num : nums)
    {
        acc = std::lcm(acc, num);
    }

    return acc;
}

// this just works because the lcm of all numbers is a multiple of instructions.size()
// as i already spent way too much time on this, i will accept it
long count_steps_to_z(std::vector<std::shared_ptr<node>> end_with_a, std::string_view instructions)
{
    std::vector<long> accs;
    long current_acc{1};
    while (true)
    {
        for (const auto instruction : instructions)
        {
            for (std::size_t i{0}; i < end_with_a.size(); ++i)
            {
                auto &cur_node{end_with_a.at(i)};
                
                switch (instruction)
                {
                case 'L':
                    cur_node = cur_node->left;
                    break;
                case 'R':
                    cur_node = cur_node->right;
                    break;
                default:
                    break;
                }

                if(cur_node->val.ends_with('Z'))
                {
                    accs.push_back(current_acc);
                    end_with_a.erase(end_with_a.begin() + i);
                    --i;
                }
            }
            ++current_acc;

            if(end_with_a.empty())
            {
                return compute_lcm(accs);
            }
        }
    }
    return 0;
}

long do_task2(const std::vector<std::string> &lines)
{

    std::vector<std::shared_ptr<node>> end_with_a;

    for (auto iter{lines.begin() + 2}; iter != lines.end(); ++iter)
    {
        auto line{*iter};
        if (line.at(2) == 'A')
        {
            std::vector<std::shared_ptr<node>> stack;
            std::shared_ptr<node> tree{build_tree({lines.begin() + 2, lines.end()}, line.substr(0, 3), stack)};
            end_with_a.push_back(tree);
        }
    }

    return count_steps_to_z(end_with_a, lines.at(0));
}

std::pair<long, long> execute_tasks()
{
    std::pair<long, long> to_return;
    std::vector<std::string> lines{parse_file_line("../input/day8")};

    std::vector<std::shared_ptr<node>> stack;
    std::shared_ptr<node> tree{build_tree({lines.begin() + 2, lines.end()}, "AAA", stack)};

    to_return.first = do_task1(tree, lines.at(0));

    to_return.second = do_task2(lines);

    return to_return;
}

int main()
{
    const auto results{execute_tasks()};

    std::cout << "First star: " << results.first << ", Second Star: " << results.second << std::endl;
}