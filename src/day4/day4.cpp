#include "utility.h"
#include <regex>
#include <unordered_map>
#include <iostream>

int parse_input(const std::string &line)
{
   std::regex reg{R"(Card \d+:|\d+|\|)"};
   std::unordered_map<int, int> map;
   int acc{0};
   auto it_start = line.begin();
   std::smatch m;

   bool card = true;
   bool my_numbers = false;

   while (std::regex_search(it_start, line.end(), m, reg))
   {
      it_start = m.suffix().first;

      if (card)
      {
         card = false;
         continue;
      }

      if (m[0].str().find("|") != std::string::npos)
      {
         my_numbers = true;
         continue;
      }

      if (!my_numbers)
      {
         map.insert({std::stoi(m[0]), 0});
      }
      else if (my_numbers && map.find(std::stoi(m[0])) != map.end())
      {
         ++acc;
      }
   }
   return acc;
}

void help_rec(const std::vector<int> &lines, int &copies, int pos)
{

   int res = lines.at(pos);

   ++copies;

   for (int i{pos + 1}; i < static_cast<int>(lines.size()) && res > 0; ++i, --res)
   {
      help_rec(lines, copies, i);
   }
}

int do_task2(std::vector<std::string> lines)
{
   std::vector<int> calls;
   int acc{0};

   for (const std::string &line : lines)
   {
      calls.push_back(parse_input(line));
   }

   for (int i{0}; i < static_cast<int>(lines.size()); ++i)
   {
      help_rec(calls, acc, i);
   }

   return acc;
}

int do_task1(std::vector<std::string> lines)
{
   int acc{0};

   for (const std::string &line : lines)
   {
      int shift_offset{parse_input(line)};
      if (shift_offset)
      {
         acc += 1 << (shift_offset - 1);
      }
   }

   return acc;
}

std::pair<int, int> execute_tasks()
{
   return {do_task1(parse_file_line("../input/day4")), do_task2(parse_file_line("../input/day4"))};
}

int main()
{
   std::pair<int, int> results{execute_tasks()};

   std::cout << "First star: " << results.first << ", Second Star: " << results.second << std::endl;
}