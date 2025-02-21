

//https://www.youtube.com/watch?v=i_wDa2AS_8w
// The purpose of this sln is to make an example of each of the 31 examples laid out in this video.
// A lot of them I hadn't even considered and were great optimisations I hadn't considered or heard of
// The examples are roughly in order. Classes are made where necessary, and discrete to each nooby habit. 
// Otherwise the example is just located in main, where available.

#include <iostream>
#include <vector>
#include <bit>
#include <memory>
#include <map>
#include "Member_Init_Order_Matters.h"
#include <cstdint>
#include <array>
#include "constexpr_sqrt.h"
#include "Function_Arg_Order.h"
#include "Timer.h"
#include "UsingNamespace.h"

#define GetMySqRt INT_MAX>>1

void constexpr_sqrt();

int main()
{
	//#1
	//Dont use using namespace std; for something like map or string: using namespace std::string; or using namespace std::map; 
	//Put it within the scope, where available. 
	UsingNamespace example1; //F12 me

	//#2
	std::cout << "Using a \\n just makes a new line. Using " << std::endl << " std::endl flushes the buffer. This takes time.\n";

	//#3
	std::vector<int> VecOfInts(10, 3); //Vector, initialized with size 10, containing the number 3 for each of those 10
	//Use a range based for loop instead of iterator when you want to perform a simple operation on each element
	for (int& i : VecOfInts)
		i++;
	std::cout << "Should say 4: " << VecOfInts[0] << '\n';

	//#4
	//Use built in functionality. Know your tools.
	//In this case we define a lambda, use that function/lambda in this find_if. I guess it iterates until it gets a true returned. Neat
	const auto is_positive = [](const int& x) { return x > 0; };
	auto first_pos_int = std::find_if(VecOfInts.begin(), VecOfInts.end(), is_positive);
	std::cout << "This is the first positive int in the vec: " << *first_pos_int << '\n';

	//#5
	//Try not to use c-style array
	int C_Style_arr[10]{}; //Size is not able to be retrieved. Need to know. (We COULD use sizeof)
	//weird, std::array LOOKED fine but I hadnt included <array> so it gave me a weird error. 
	std::array<int, 10> std_arr{ {0} };
	std::cout << "Array size: " << std_arr.size() << '\n';

	//#6
	//Reinterpret_cast - Don't use reinterpret cast to totally change an object type for whatever purposes.
	//bit_cast is C++ 20.
	//This example is different from the video. He uses a templated function in a cool way. 
	constexpr std::uint64_t u64v2 = 0x3fe9000000000000ull;
	constexpr auto f64v2 = std::bit_cast<double>(u64v2);

	//#7
	//Dont cast away const. wtf yeah. 
	//#8
	// map[element] will insert into the map. It's not a lookup like a vector. 
	//#9 
	//Use const correctness

	//#10
	//String literals last the lifetime of the program. 
	//Somewhat wrong. They a constant just like returning 10 from a function. They live within their scope, on the stack. 
	//So returning "Hello World" from a function, caugt by a const char * will last as long as that const char * variable is alive.

	//#11
	//Structured binding.
	std::map<string, string> colors = { {"RED", "#FF0000"}, {"BLUE", "#00FF00"}, {"GREEN", "#0000FF"} };
	for (const auto& [name, hex] : colors)
		std::cout << "Name: " << name << ", Hex: " << hex << '\n';
	//We COULD just iterate the map and use map->first and second but this is nice and readable. Very cool. Looks like its just sort of temp casting the name
	//He had other structured binding examples for structs and stuff. Neat.

	//#12
	//He argues you shouldnt pass by ref for multiple return values but make a struct that is returned. 
	//HARD Disagree. Now we need to know about this type and catch a copy to use the values. No.

	//#13
	//using constexpr to calculate at compile time. Cool. I have the constexpr_sqrt below, for usage and a class for it constexpr_sqrt.h

	//#14
	//Use virtual destructor so it can be called properly when a pointer to the object is of a different type. 

	//#15
	Member_Init_Order_Matters MemberInitializerOrder(1,99);
	std::cout << "Second num should be 1+1: " << MemberInitializerOrder.GetSecondNum() << " But its garbage\n";
	//His conclusion is to fix the order of the values but my opinion is that if a value needs to be calculated, do it in the constructor body not initialize list

	//#16
	//Initialize values.
	int Sixteen1; //Obviously not initialized Gives warning.
	int Sixteen2{}; //Initialized to 0
	int Sixteen3 = 0; //Initialized to 0
	//int Sixteen4(); //This is the syntax for a func declaration

	//#17
	constexpr float SPEED_OF_LIGHT = 123456.0f; //better than inline magic number. constexpr is like a local #define

	//#18
	//DANGEROUS code below. We are iterating through the vector, adding to it from itself. This is semi-recursive as you get to the end of the vec, it grows.
	//An "official" iterator would do the same, still. 
	//	for (auto iter : VecOfInts)
	//		VecOfInts.push_back(iter);
	//So, funny, the solution is the tried and true for loop where we use vector[i]. We just have to save the .size before looping, to check against, since size will increase

	//#19
	// 8:30
	//Actually unsure of his point here. I think there is a lot of confusion OTHER people have with move. 
	//I dont have this confusion so I can't follow what the correction to other peoples confusion is. I've seen this twice now
	//https://www.youtube.com/watch?v=9mWWNYRHAIQ and https://youtu.be/6SaUwqw4ueE?si=mbhSP7YYzrrzrISF
	//This first link, I THINK, is an expanded version of #19 as far as I can tell.
	//If you need to fill out an array in a func, I think youre going to be forced to send in a ref to the array and do it. Seems fine to me.

	//#20
	//Yeah move doesnt do some magic removal of a data copy. Only a pointer can do that the way people think.

	//#21
	//function argument order is not guaranteed? WOW don't think I knew this
	std::cout << Function_Arg_Order::ReceiveSomeInputAndReturnCalc(
		Function_Arg_Order::Calc(2), Function_Arg_Order::Calc(3), Function_Arg_Order::Calc(4));
	//These three Calc calls can happen in any order, so if they were vars that were dependant on eachothers value in the calc, you need to pre-calc that.
	//OR, in this case, inside ReceiveSomeInputAndReturnCalc, call the Calc() for them.

	//#22
	//Dont just put stuff on the heap. No shit

	//#23
	auto TimerPtr = std::make_unique<Timer>(Timer());
	//Use smart pointers so we dont have to make sure delete is in every branch. Sure. 
	//Uniques are cheaper because of the cost of ref counting so use when possible. 

	//#24
	//He annoyingly used auto Whatver = std::unique_ptr in his version of 23 instead of make_unique. 
	//The make version we used is better by sending the data to the constructor directly. wtf

	//#25
	//Basically says never call new directly... dude chill

	//#26
	//Try to use RAII - Resource Acquisition is Initialization
	//Has a good example of using an ifstream instead of FILE.open and .close. The ifstream closes itself when destructed when falling out of scope.

	//#27
	//Obviously its fine to use RAW pointers as a way to pass large objects or whatever in a function. 
	//If you use malloc, you must use free.

	//#28
	//More cruff about using a unique ptr. Let the user convert it to shared when they need to, which can be done with move, looks like
	auto TimerPtrUniq = std::make_unique<Timer>(Timer());
	std::shared_ptr<Timer> TimerPtrShared = std::move(TimerPtrUniq);

	//#29
	//Who thinks a smart pointer is thread safe without locks? 

	//#30
	int x = 0;
	const int* ptr1 = &x; //Pointer to a const int
	int const* ptr2 = &x; //Pointer to a const int
	int* const ptr3 = &x; //Const pointer to an int (int is not const)
	const int* const ptr4 = &x; //Const pointer to a const int.

	//#31
	//Dont ignore compiler warnings. Can result in undefined behavior. 


	return 0;
}

void constexpr_sqrt()
{
	//Calculation is off by 1
	Timer timer1;

	std::cout << "Square root of " << (GetMySqRt) << " with const r - value: " << ct_sqrt(GetMySqRt) << " ";
	timer1.Reset();
	ct_sqrt(GetMySqRt);
	timer1.Stop(false);
	std::cout << "Time: " << timer1.GetTimeSpent() << "\n";

	//With input
	char sqrt_input_char[20];
	std::cin.get(sqrt_input_char, 20);
	//How are we able to use the sqrt call of a constexpr with variable input? I thought it would not work.
	//Does the constexpr just calculate as needed and pre-calculate and build what it can?
	int sqrt_input = atoi(sqrt_input_char);
	timer1.Reset();
	sqrt_input = ct_sqrt(sqrt_input);
	timer1.Stop(false);
	std::cout << "constexpr sqrt with variable input: " << sqrt_input << " Time: " << timer1.GetTimeSpent() << "\n";

	// timer1.Reset();
	// sqrt_input = sqrt((int)GetMySqRt);
	// timer1.Stop(false);
	// std::cout << "Standard sqrt function: " << sqrt_input << " Time: " << timer1.GetTimeSpent() << "\n";
}
