//
// Created by sluggernot on 10/28/24.
//

#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct CustomType
{
  std::string name;

  bool operator < (const CustomType &other) const
  {
    return name < other.name;
  }
  bool operator == (const CustomType &other) const
  {
    return name == other.name;
  }
};

//std::hash<std::string> hasher;

int main()
{
  //std::map - Uses the < to make a balanced binary tree. Unique values only, like a set. O(Log n) for insert, find, delete
  //std::unordered_map - Uses the hash to balance and order the tree. O1 search and shit.
  //std::multimap - sorted list of key-value pairs. Allows multiple entries with the same key. Unsure how to find additional elements with same key.
  //std::unordered_multimap - unsorted list and allows multiple entries with the same key.
  //std::flat_map<std::string, std::string> flatMap - Not std for real. Sorted vector (also flat_multimap)

  // std::set<std::string> keys;//One copy of each element. Ordered by <.
  // std::multiset<std::string> multiset; //multiple copies of each element. Ordered by <.
  // std::pmr::unordered_set<std::string> unorderedset; //One copy of each element. Ordered by hash.
  // std::pmr::unordered_multiset<std::string> unordered_multiset; //Multiple copies of each element. Ordered by hash.

  std::map<std::string, std::string> basicMap;
  basicMap["USA"] = "Washington DC";
  basicMap["Japan"] = "Tokyo"; //Kyoto?
  basicMap["Mexico"] = "Mexico City";
  basicMap["Australia"] = "Camembury or some shit";//Just creates the value and overwrites old one if exists.
//  firstMap.emplace({"Russia", "Stalingrad"}, "Russia");//Didn't work? Would have sworn. So how do you use emplace?
  basicMap.insert({"Russia", "Stalingrad"});//Creates the std::pair like below just not explicit.
  basicMap.insert(std::make_pair("Russia", "Stalingrad"));//Same as above
  //These two below appear to be the most efficient, NOT creating the std::pair, just inserting.
  basicMap.try_emplace("Russia", "Stalingrad"); //Only adds if Key doesnt exist
  basicMap.insert_or_assign("Russia", "Stalingrad"); //Will overwrite if key does exist already

  std::cout << basicMap["USA"] << std::endl;
  std::cout << basicMap["Japan"] << std::endl;
  std::cout << "\nIterating basic map: \n";

  for(auto& Country : basicMap)
  {
    std::cout << Country.first << " " << Country.second << std::endl;
  }
  std::cout << "\n\n";

  std::unordered_map<std::string, std::string> unorderedMap;
  unorderedMap["USA"] = "Washington DC";
  unorderedMap["Japan"] = "Tokyo";
  unorderedMap.insert({"Russia", "Stalingrad"});
  unorderedMap.insert_or_assign("Russia", "Stalingrad");
  unorderedMap.emplace("Russia", "Stalingrad");
  unorderedMap.try_emplace("Australia", "Canderbury");

  std::cout << unorderedMap["USA"] << std::endl;
  std::cout << "\nIterating unordered map: \n";
  for(auto& Country : unorderedMap)
  {
    std::cout << Country.first << " " << Country.second << std::endl;
  }
  std::cout << "\n\n";

  std::unordered_multimap<std::string, std::string> unorderedMultimap;
  unorderedMultimap.insert({"USA", "Washington DC"});
  unorderedMultimap.insert({"Spain", "Barcelona City"});
  unorderedMultimap.insert({"Portugal", "Lisbon"});
  unorderedMultimap.insert({"England", "London"});
  //unorderedMultimap["USA"] = "Washington DC"; //Illegal entry type
  unorderedMultimap.emplace("Russia", "Stalingrad");
  unorderedMultimap.emplace("Russia", "Leningrad");
  std::cout << "\nIterating unordered multimap: \n";
  for (auto& Country : unorderedMultimap) //auto& [fst, snd]
  {
    std::cout << Country.first << " " << Country.second << std::endl;
  }
  std::cout << "Russia has two entries. It appears the first one entered into the map is returned (first)\n";
  auto Country = unorderedMultimap.find("Russia");
  std::cout << Country->first << " " << Country->second << std::endl;
  std::cout << "Iterating all results with \"Russia\" as key: \n";
  auto countryIter = unorderedMultimap.equal_range("Russia");
  for(auto iter = countryIter.first; iter != countryIter.second; ++iter)
  {
    std::cout << iter->first << " " << iter->second << std::endl;
  }


  return 0;
}
