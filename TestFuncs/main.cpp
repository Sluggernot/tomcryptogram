#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <set>

#include <future>


using namespace std;

//https://riptutorial.com/cplusplus/example/11738/types-of-maps

int fast_modulo(int num, int mod)
{
	//mod MUST be power of 2 for this to work. Neat!
	return num & (mod - 1);
}

//https://leetcode.com/problems/two-sum/ One of the fastest answers. 
//Given an array of integers nums and an integer target, return indices of the two numbers such that they add up to target.
vector<int> twoSum(vector<int>& nums, int target) {
	//Creating a map for O(1) lookup
	unordered_map<int, int> um;
	for (int i = 0; i < nums.size(); i++) {

		if (um.find(target - nums[i]) != um.end()) {
			return { um[target - nums[i]], i };
		}

		um[nums[i]] = i;

	}
	return {};
}

unsigned int FindThisMany(const std::vector<int>& nums, int startIndex, bool forward, int uniqueCount) {

	std::unordered_set<int> uniques;
	unsigned int minLength = nums.size();
	int left = startIndex, right = startIndex;

	if (forward) {
		while (right < nums.size() && uniques.size() < uniqueCount) {
			uniques.insert(nums[right]);
			right++;
		}
	}
	else {
		while (left >= 0 && uniques.size() < uniqueCount) {
			uniques.insert(nums[left]);
			left--;
		}
	}

	if (uniques.size() == uniqueCount) {
		minLength = forward ? right - startIndex : (startIndex - left);
	}

	return minLength;
}

int smallestSubarrayWithAllUniqueNumbers3(const std::vector<int>& nums)
{
	//Only needs 1 multimap
	std::unordered_multimap<int, int> uniquePtrs;
	for (int i = 0; i < nums.size(); ++i) {
		uniquePtrs.emplace(nums[i], i);
	}

	//Could it be advantageous to record multiple 
	auto smallestContainer = std::min_element(uniquePtrs.begin(), uniquePtrs.end(),
		[&](const auto& a, const auto& b) { return uniquePtrs.count(a.first) < uniquePtrs.count(b.first); });

	int lowestLength = nums.size();
	std::vector<std::future<unsigned int>> asyncCalls;
	auto range = uniquePtrs.equal_range(smallestContainer->first);
	for (auto it = range.first; it != range.second; ++it) {
		asyncCalls.push_back(std::async(&FindThisMany, std::cref(nums), it->second, true, uniquePtrs.count(it->first)));
		asyncCalls.push_back(std::async(&FindThisMany, std::cref(nums), it->second, false, uniquePtrs.count(it->first)));
	}

	for (int i = asyncCalls.size() - 1; i >= 0; --i) {
		int length = asyncCalls[i].get();
		if (length < lowestLength) {
			lowestLength = length;
		}
	}

	return lowestLength;
}
/*
int smallestSubarrayWithAllUniqueNumbers2(const std::vector<int>& nums) 
{
	int uniqueCount = 0;
	std::unordered_multimap<int, int*> uniquePtrs;
	for (int i = 0; i < nums.size(); ++i)
	{
		uniquePtrs.emplace(nums[i],  &nums[i]);
	}

	{
		std::unordered_set<int> uniqueNumbers(nums.begin(), nums.end());
		int uniqueCount = uniqueNumbers.size();
	}

	std::unordered_map<int, int> counts;
	for (int num : nums) {
		counts[num]++;
	}

	int mostUniqueNumber = nums[0];
	{
		int minCount = counts[mostUniqueNumber];
		for (const auto& mypair : counts) {
			if (mypair.second < minCount) {
				mostUniqueNumber = mypair.first;
				minCount = mypair.second;
			}
		}
	}

	std::vector<int> mostUniqueIndices;
	for (int i = 0; i < nums.size(); ++i) {
		if (nums[i] == mostUniqueNumber) {
			mostUniqueIndices.push_back(i);
		}
	}

	int lowestLength = nums.size();
	std::vector<std::future<int>> asyncCalls;
	for (int index : mostUniqueIndices) {
		if(nums.size()-index >= uniqueCount) // only add thread if there is a possibility for the iteration to contain the uniques
		asyncCalls.push_back(std::async(&FindThisMany, std::cref(nums), index, true/*right, uniqueCount));
		if(index > uniqueCount) //if we're early in the array, don't go backwards.
		asyncCalls.push_back(std::async(&FindThisMany, std::cref(nums), index, false/*left, uniqueCount));
	}
	//Doesnt have to be a reverse for loop. Probably shouldn't be since the first async spawned has calculated the longest.
	for (int i = 0; i < asyncCalls.size(); ++i) {
		int length = asyncCalls[i].get();
		if (length < lowestLength) {
			lowestLength = length;
		}
	}

	return lowestLength;
}
*/

int GetlocationOfInt(vector<int>& vec)
{
	//Lets say our data set is a shitload of 1 2 and 3 and a few 7s.
	//Calculate the number of unique numbers with the unordered_set
	//Find the number in the vec that is most unique.(7 in my case)
	//From each 7, search forward and back, finding how far we have to go to encapsulate all unique numbers
	//return lowest number
	int LowestCountInt = 4;
	int LowestLength = vec.size();

	std::vector<std::future<int>> asyncCalls;
	for (int i = 0; i < LowestCountInt; i++)
	{
		//Check that there is even room foward and backward 
		//asyncCalls.push_back(std::async(&FindThisMany, i, true));
		//asyncCalls.push_back(std::async(&FindThisMany, i, false));
	}
	//Doing a reverse for loop in a case where I think the later futures would take less time.
	for (int i = asyncCalls.size() - 1; i >= 0; --i)
		if (asyncCalls[i].get() < LowestLength) LowestLength = asyncCalls[i].get();

	return 0;
}
/*
const auto is_positive = [](const int& x) { return x > 0; };
auto first_pos_int = std::find_if(VecOfInts.begin(), VecOfInts.end(), is_positive);
std::cout << "This is the first positive int in the vec: " << *first_pos_int << '\n';
*/

int smallestSubarrayWithAllUniqueNumbers(const std::vector<int>& nums) 
{
	int uniqueCount = 0;
	{
		std::unordered_set<int> uniqueNumbers(nums.begin(), nums.end());
		uniqueCount = uniqueNumbers.size();
	}//Quick scope to pop this off of the stack after getting the count.

	std::unordered_map<int, int> windowCounts;
	int left = 0, right = 0;
	int minLength = nums.size() + 1;
	int currentUniqueCount = 0;

	while (right < nums.size()) {
		//init this int to first element in vector
		int rightNum = nums[right];
		//Insert an element to the map and/or add 1 to it
		windowCounts[rightNum]++;
		//if this is our first time seeing this number, add to uniqueCount
		if (windowCounts[rightNum] == 1) {
			currentUniqueCount++;
		}
		//Once we have our unique count found, starting from the left, now go from the right
		while (currentUniqueCount == uniqueCount) {
			minLength = std::min(minLength, right - left + 1);
			int leftNum = nums[left];
			windowCounts[leftNum]--;
			if (windowCounts[leftNum] == 0) {
				currentUniqueCount--;
			}
			left++;
		}
		right++;
	}

	return minLength;
}

//Arbitrary work for async future example.
int square(int x) { return x * x; }

int lengthOfLongestSubstring(string s) {
	unordered_set<char> chars;

	int best = 1;
	for (unsigned int i = 0; i < s.length() - 1; ++i)
	{
		chars.emplace(s[i]);
		for (unsigned int ii = i + 1; ii < s.length(); ++ii)
			if (chars.find(s[ii]) != chars.end()) //chars.contains(s[i]))
			{
				if (chars.size() > best) best = chars.size();
				chars.clear();
				break;
			}
			else chars.emplace(s[ii]);
	}
	//if we ended iterating and had the best size
	if (chars.size() > best) return chars.size();
	return best;
}

void CalculateAllSums(vector<int>& nums, set<int>& setofInts, int n)
{
	for (int i = 0; i < nums.size(); ++i)
	{
		setofInts.emplace(nums[i]);
		
		int jump = 0;
		while (jump++ < nums.size())
		{
			int temp = nums[i];
			for (unsigned int addIt = i + jump; addIt < nums.size(); addIt += jump)
			{
				temp += nums[addIt];
				if (temp < n)
					setofInts.emplace(temp);
				else break; //dont just keep exceeding n
			}
		}
	}
}
int minPatches(vector<int>& nums, int n) {
	//wth is this problem? afaik, they want to know what numbers need to be added to the array
	//so that every number in the array has a matching number(s) that adds up to n, including n
	int originalSize = nums.size();
	//I'm thinking this is an insane problem... especially since you can apparently have duplicate numbers. dude wtf
	vector<int> patches;
	set<int> setofInts;
	setofInts.emplace(n); //make sure our end is created. 
	CalculateAllSums(nums, setofInts, n);
	int isMissing = 1;
	auto it = setofInts.begin();
	for (; it != setofInts.end(); ++it)
	{
		if (setofInts.find(isMissing) == setofInts.end())
		{
			nums.push_back(isMissing);
			setofInts.emplace(isMissing);
			CalculateAllSums(nums, setofInts, n);
			cout << isMissing << ' ';
			//it = setofInts.begin();
		}
		++isMissing;
	}
	return nums.size() - originalSize;
}

string QuickTernaryTest()
{
	string s("A");
	string t("T");
	return true ? s : t + "+" + s;
}

int main()
{
	cout << "Quick Ternary Test: " << QuickTernaryTest() << endl;


	vector<int> nums{ 1,5,10 };
	minPatches(nums, 20);

	lengthOfLongestSubstring("pwwkew");

	std::vector<int> nums2 = { 1, 1, 2, 1, 1, 3, 2, 2, 2, 1 };
	int result = smallestSubarrayWithAllUniqueNumbers(nums2);
	std::cout << "The size of the smallest subarray that encapsulates all unique numbers is: " << result << std::endl;

	//My own advanced interpretation:
	std::vector<std::future<int>> asyncCalls;
	const int x = 20;
	for (int i = 0; i < x; i++)
	{
		asyncCalls.push_back(std::async(&square, i));
	}
	//Doing a reverse for loop in a case where I think the later futures would take less time.
	for (int i = asyncCalls.size() - 1; i >= 0; --i)
	{
		cout << asyncCalls[i].get() << ' ';
	}
	cout << endl;


	return -1;
}
