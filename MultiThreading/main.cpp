
#include <iostream>
#include <vector>
#include <thread>
//#include <jthread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <random>
#include <future>

using namespace std;
using namespace std::literals::chrono_literals;
using namespace std::this_thread;

#define ALL_TESTS 0
#define TEST1 0
#define TEST2 0

//went through the tutorial again, 6-8-2024
//https://www.youtube.com/watch?v=Fn0xBsmact4&list=PLvv0ScY6vfd_ocTP2ZLicgqKnvq50OCXM 
#define REDO_CODE 1

//Doesnt need to be atomic?
static bool sg_Finished = false;
static int sg_LockInt = 0;
mutex g_lock;
condition_variable gConVar;

void DoWork()
{
	//Weird. If I dont make this atomic, it doesnt work. 
	//Doesnt seem to make sense to me because it's contained withing the thread. 
	//Is this multiple threads, doing this work? I'm missing key info. 
	//atomic<int> count = 0;
	unsigned int count = 0;
	//holy shit. the int count is wrapping back around every second or so.
	while (!sg_Finished)
	{
		++count;
		if (count < 100)
			cout << count << " ";
		else
			sg_Finished = true;
		//if (count > 20 && count < 30)
		//{
		//	cout << "ThreadID: " << std::this_thread::get_id << endl;
		//}
		std::this_thread::sleep_for(.1s);
	}
}

void ArbitraryWork(int x = 0)
{
	//cout << "Thread ID: " << std::this_thread::get_id;
	for (int i = 0; i < x; ++i)
	{
		if (i % 1000000 == 0)
			cout << i << " ";
	}
	cout << endl;
}

void LockExample()
{
	int randInt = 1000000 + rand() % 1000000;
	cout << "RandInt: " << randInt << "  ";
	ArbitraryWork(randInt);
//	g_lock.lock();
	{
		//Just needs to be placed within some scope and unlocks when out of scope/destroyed.
		//could be placed at the bottom of this function, not in this scope, of course.
		//removes need to have mutex.unlock() in a lot of places
		lock_guard<mutex> lockGuard(g_lock);
		++sg_LockInt;
	}
// 	g_lock.unlock();
}

//Forward Declaring functions from below main
void StdAsync();
void CheckingIfAsyncIsDone();

int main()
{
	srand((unsigned)time(0));
	cout << "Main thread id: " << std::this_thread::get_id << endl;

#if REDO_CODE || ALL_TESTS
	//StdAsync();
	CheckingIfAsyncIsDone();

	return 0;
#endif

#if TEST1 || ALL_TESTS
	//Basic thread with actual function
	{
		//initialize a thread with a function pointer.
		cout << "ENTRY POINT ";
		thread worker(DoWork);
		//Easier to see if I sleep the main thread, that simply spawning the worked thread starts the work. 
		std::this_thread::sleep_for(1s);
		cout << "THREAD CREATED ";
		//Define input catcher, that waits, on main thread. 
		std::cin.get();
		//We wait here, forever, until input is received. Main thread is waiting on input. worker thread is working.
		cout << "INPUT RECEIVED ";
		//When input is received, set Finished to true for thread to read.
		//sg_Finished = true;

		//If work were still happening on the worker thread, we would pause here. 
		worker.join();
		cout << "THREAD JOINED ";
		cout << endl << endl;
	}
#endif //TEST1 || ALL_TESTS

	//reset our bool
	sg_Finished = false;

#if TEST2 || ALL_TESTS
	//Lambdas and jthreads.
	{
		auto myLambda = [](int x) { cout << "Thread ID: " << std::this_thread::get_id; cout << " Num: " << x << endl; };
		thread Lambda1(myLambda, 99);
		Lambda1.join();

		auto myLambdaFuncCall = [](int x) { ArbitraryWork(x); };
		thread Lambda2(myLambdaFuncCall, 1000001);
		Lambda2.join();

		cout << "About to spawn jthreads\n";
		vector<jthread> jthreads;
		for (int i = 0; i < 10; ++i)
		{
			jthreads.push_back(jthread(myLambdaFuncCall, i * 100000 + 1));
		}
	}
#endif //TEST2 || ALL_TESTS

	//https://www.youtube.com/watch?v=XZDx41woDoI&list=PLvv0ScY6vfd_ocTP2ZLicgqKnvq50OCXM&index=10
	bool notified = false;
	thread reporter([&] {
		cout << "First Lambda" << endl;
		//Inline Lambda function body
		unique_lock<mutex> lock(g_lock);
		if (!notified)
		{
			//This never prints but no idea why. IT WAS BECAUSE IM NOT FLUSHING THE BUFFER (can use endl to do so)
			//Using \n is totally different in its behavior with the print to the cmd
			cout << "Before wait" << endl;
			gConVar.wait(lock);
			cout << "After wait" << endl;
		}
		cout << "We waited based on both the notified bool and the lock/condition var\n" << endl;

		} );
	thread worker([&] {
		cout << "Second Lambda" << endl;
		//Inline Lambda function body
		unique_lock<mutex> lock(g_lock);
		//Do some work.
		notified = true;
		std::this_thread::sleep_for(3s);
		cout << "Sleep over\n";
		gConVar.notify_one();
		} );

	/*
	* Main thread work that has to be on the main thread for memory reasons
	*/

	worker.join();
	reporter.join();


	return 0;
}

void MostBasicPossibleThread()
{
	//https://www.youtube.com/watch?v=Q7gqDzOFO7E&list=PLvv0ScY6vfd_ocTP2ZLicgqKnvq50OCXM&index=3
	//Really, the most basic thread would probably have the lambda defined in its creation and have no input to the lambda func.

	auto lambda = [](int x) {
		cout << "ThreadID: " << this_thread::get_id << "\n Print int: " << x << "\n";
	};

	thread myThread(lambda, 100);
	myThread.join();
	
	//I think the use for this would be VectorOfthreads below
}

void VectorOfThreads()
{
	//https://www.youtube.com/watch?v=kfLE57ljoEE&list=PLvv0ScY6vfd_ocTP2ZLicgqKnvq50OCXM&index=4

	auto lambda = [](int x) {
		cout << "ThreadID: " << this_thread::get_id << "\n Print int: " << x << "\n";
	};

	vector<std::thread> threads;
	for (int i = 0; i < 10; ++i)
	{
		//honestly, I see now how using std:: ensures the reader knows this is a std::thread object and not some variable name and crap. Nice.
		threads.push_back(std::thread(lambda, i));
	}
	for (int i = 0; i < 10; ++i)
	{
		threads[i].join();
	}

	//This WILL run concurrently but there are no blocks or anything so it just prints all over the place. 
	// Still good, if the threads were made to be independent and just do their own stuff like load files. 
}

void jThreadExample()
{
	//https://www.youtube.com/watch?v=AhPhYvFphBA&list=PLvv0ScY6vfd_ocTP2ZLicgqKnvq50OCXM&index=5
	//Unsure why I removed jthread include. Do this example later? 
	//JThreads just join automatically when their destructor is called (when they leave scope. C++20) Not sure if I care. Cool but... meh?
}

//Part of below example
static int shared_value_inc = 0;
//Mutex = mutual exclusion (also called a binary semaphore)
mutex gLock;
void shared_value_increment()
{
	gLock.lock();
		//Use indentation within lock to make it clear.
		shared_value_inc += 1;
	gLock.unlock();
}
void PreventRaceConditions()
{
	//https://www.youtube.com/watch?v=hXKtYRleQd8&list=PLvv0ScY6vfd_ocTP2ZLicgqKnvq50OCXM&index=6
	vector<std::thread> threads;
	for (int i = 0; i < 10; ++i)
	{
		//honestly, I see now how using std:: ensures the reader knows this is a std::thread object and not some variable name and crap. Nice.
		threads.push_back(std::thread(shared_value_increment));
	}
	for (int i = 0; i < 10; ++i)
	{
		threads[i].join();
	}
	cout << "Shared Val: " << shared_value_inc << "\n";
}


void UseLockGuard()
{
	//https://www.youtube.com/watch?v=Zp17-UDKM90&list=PLvv0ScY6vfd_ocTP2ZLicgqKnvq50OCXM&index=7
	//using the mutex gLock from above, in global scope, and controlling its unlock by having this lock_guard just fall out of scope.
	//Good for handling multiple returns in the work section
	lock_guard<mutex> lockGuard(gLock);
	//DO WORK HERE
	// 
	//There is also a scoped_lock which he says is preferable? I wonder wtf is the difference. This is very compact.  
}

//Ok these are for the primitive data types. I guess they're thread safe, by their construction. They specifically have the ++ overloaded an stuff like that
//In his example, he does atomInt = atomInt + 1 and it DOES NOT work in a locked thread safe manner. 
atomic<int> atomInt = 0;
void Atomics()
{
	//https://www.youtube.com/watch?v=f_C4eYxBWdQ&list=PLvv0ScY6vfd_ocTP2ZLicgqKnvq50OCXM&index=8
	atomInt++; //Thread safe. Something to do with small number of bits. 
	atomInt = atomInt + 1; // NOT THREAD SAFE. NOT OVERLOADED IN C++ DEFINITION. 
	//Ok, so atomics with the standard types is well defined and if you need some tiny calculations with them you can use them across threads.
	//https://en.cppreference.com/w/cpp/atomic/atomic
	//atomInt.is_lock_free() //checks if the atomic object is lock-free? There are interesting functions with these that can make them act like a lock with a condition. Neat.
}

int square(int x) { return x * x; }

void StdAsync()
{
	//https://www.youtube.com/watch?v=4twJD5ezkag&list=PLvv0ScY6vfd_ocTP2ZLicgqKnvq50OCXM&index=11
	future<int> asyncFunc = std::async(&square, 12);
	//.get is a blocking operation. Same as join. 
	int asyncResult = asyncFunc.get();
	cout << "AsyncFunc result: " << asyncResult << "\n";

	///////////////////////////////////////////////////////////////////////////////////////////////

	//My own advanced interpretation:
	const int x = 20;
	std::vector<std::future<int>> asyncCalls(x); //Make x number of futures.
	for (int i = 0; i < x; i++)
	{
		asyncCalls.push_back(std::async(&square, i));
	}
	//Doing a reverse for loop in a case where I think the later futures would take less time.
	for (int i = asyncCalls.size()-1; i >= 0; --i)
	{
		cout << asyncCalls[i].get() << ' ';
	}
	cout << endl;
}

int workSimForBelow()
{
	int bytesLoaded = 0;
	while (bytesLoaded < (INT_MAX >> 1))
	{
		if (bytesLoaded % 1000000 == 0) cout << "BG Thread working\n";
		++bytesLoaded;
	}

	return bytesLoaded;
}

void CheckingIfAsyncIsDone()
{
	//https://www.youtube.com/watch?v=2eJm6tA4y2U&list=PLvv0ScY6vfd_ocTP2ZLicgqKnvq50OCXM&index=12
	//Ok, looks almost exactly like the last example BUT the idea is that we can continue doing stuff but each frame check if our task is complete.
	std::future<int> backgroundThread = std::async(std::launch::async, &workSimForBelow);

	std::future_status status;
	while (true)
	{
		std::cout << "Main thread is running: \n";
		std::this_thread::sleep_for(chrono::milliseconds(50));
		status = backgroundThread.wait_for(chrono::milliseconds(1));
		//if backgroundThread is completed, break;
		if (status == future_status::ready)
		{
			cout << "Background work complete: " << backgroundThread.get() << endl;
			break;
		}
	}
}


#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
	ThreadPool(size_t numThreads) : numThreads_(numThreads), mainThreadId_(std::this_thread::get_id()), activeThreads(0) {
		for (size_t i = 0; i < numThreads_; ++i) {
			threads_.emplace_back([this] {
				while (!stop_) {
					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lock(queueMutex_);
						cv_.wait(lock, [this] { return !tasks_.empty() || stop_; });
						if (stop_) {
							break;
						}
						task = std::move(tasks_.front());
						tasks_.pop();
					}
					task();
					activeThreads--;
				}
				});
		}
	}

	~ThreadPool() {
		{
			std::unique_lock<std::mutex> lock(queueMutex_);
			stop_ = true;
		}
		cv_.notify_all();
		for (auto& thread : threads_) {
			thread.join();
		}
	}

	void join()
	{
		//Dangerous to be cyclic? Use the thread worker example in main?
		while (activeThreads > 0);
	}

	void enqueue(std::function<void()> task) {
		activeThreads++;
		{
			std::unique_lock<std::mutex> lock(queueMutex_);
			tasks_.push(std::move(task));
		}
		cv_.notify_one();
	}

private:
	size_t numThreads_;
	std::mutex queueMutex_;
	std::condition_variable cv_;
	std::atomic<int> activeThreads;
	bool stop_ = false;
	std::thread::id mainThreadId_;
	std::vector<std::thread> threads_;
	std::queue<std::function<void()>> tasks_;
};

/*
pool.enqueue([this, arg1, arg2, arg3] {
	Class::Work(arg1, arg2, arg3);
});

potentiall save std::thread::id as a data member and assert or something if incoming thread id doesnt match main thread id.
member initialize list : numThreads_(numThreads), mainThreadId_(std::this_thread::get_id())
This would prevent threads from accessing enqueue at the same time, race conditions, etc. 
if (std::this_thread::get_id() != mainThreadId_) {
			throw std::runtime_error("Tasks can only be added from the main thread");
*/