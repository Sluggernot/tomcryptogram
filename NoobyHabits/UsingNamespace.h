#pragma once

#include <map>
#include <string>

//Can NOT be defined inside the class? Weird
using std::string, std::map;
//The above statement is for C++ 17 and above. Before that, these would need to be declared 1 per line.

class UsingNamespace
{
public:

	map<int, float> myMap;
	string exampleString;
};

