cmake_minimum_required(VERSION 3.25)
project(CLion_Test1)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(MagicSquares)
add_subdirectory(PowersOfFive)
add_subdirectory(EulerBrick)

#Weird. Couldnt name the exe the same as the folder.
add_executable(EveryMapout EveryMap/main.cpp)

add_executable(FiveMinuteTimer FiveMinuteTimer/fiveminute.cpp)

#add_executable(NoobyHabits NoobyHabits/main.cpp)
add_subdirectory(NoobyHabits)

add_executable(MultiThreading MultiThreading/main.cpp)

add_executable(TestFuncs TestFuncs/main.cpp)

