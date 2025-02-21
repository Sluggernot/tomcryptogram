#pragma once

#include <chrono>
#include <iostream>

class Timer
{
	std::chrono::system_clock::time_point StartTime;
	long long TimeSpent = 0;

public:

	Timer() { StartTime = std::chrono::high_resolution_clock::now(); }
	~Timer() { Stop(false); }

	void Stop(bool Print = true)
	{
		//Immediately lock in EndTime before creating other variables or doing anything
		auto EndTime = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(StartTime).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(EndTime).time_since_epoch().count();
		TimeSpent = end - start;

		if (Print)
			std::cout << "Time in microseconds: " << TimeSpent << "\n";
	};

	void Reset() { StartTime = std::chrono::high_resolution_clock::now(); };
	long long GetTimeSpent() const { return TimeSpent; }
};

