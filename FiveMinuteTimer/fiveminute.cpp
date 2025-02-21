
#include <iostream>
#include <chrono>
#include <functional>
#include <thread>
#include <future>

using namespace std;
using namespace std::chrono_literals;

template<class _Rep, class _TimePeriod>
std::future<void> TimerAsync(std::chrono::duration<_Rep, _TimePeriod> duration, std::function<void()> callback)
{
	return std::async(std::launch::async, [duration, callback]()
		{
			std::this_thread::sleep_for(duration);
			callback();
		});
}

int main()
{
	//5min, 5ms, 5s... all works
	auto future1 = TimerAsync(5s, []() {std::cout << "Timer complete! \n"; });

	while (true)
	{
		//These two lines are our game loop
		cout << "Processing..." << endl;
		std::this_thread::sleep_for(1s);

		//Terminate program when timer is complete
		auto status = future1.wait_for(1ms);
		if (status == std::future_status::ready)
			break;
	}

	cout << "Complete!\n";

	return 0;
}