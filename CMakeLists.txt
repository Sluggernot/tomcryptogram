cmake_minimum_required(VERSION 3.28)
project(CLion_Test1)

set(CMAKE_CXX_STANDARD 20)

add_executable(CLion_Test1 src/main.cpp)
add_executable(MagicSquares MagicSquares/main.cpp)

find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(MagicSquares
        PRIVATE
        gmp
        gmpxx
        tommath
        Eigen3::Eigen)

set(Boost_USE_STATIC_RPATH ON)
find_path(Boost_Multiprecision_DIR /usr/include/boost)  # Replace with your Boost installation path
target_link_directories(MagicSquares PUBLIC ${Boost_Multiprecision_DIR})
link_directories(MagicSquares ${Boost_Multiprecision_DIR})

#Weird. Couldnt name the exe the same as the folder.
add_executable(EveryMapout EveryMap/main.cpp)

add_executable(FiveMinuteTimer FiveMinuteTimer/fiveminute.cpp)

#add_executable(NoobyHabits NoobyHabits/main.cpp)
add_subdirectory(NoobyHabits)

add_executable(MultiThreading MultiThreading/main.cpp)

add_executable(TestFuncs TestFuncs/main.cpp)

