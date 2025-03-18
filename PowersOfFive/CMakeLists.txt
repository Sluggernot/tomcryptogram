cmake_minimum_required(VERSION 3.25)
project(PowersOfFive)

set(CMAKE_CXX_STANDARD 20)

add_executable(PowersOfFive
        main.cpp
)

#find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(PowersOfFive
        PRIVATE
        gmp
        gmpxx
        #tommath
        #Eigen3::Eigen
)

set(Boost_USE_STATIC_RPATH ON)
find_path(Boost_Multiprecision_DIR /usr/include/boost)  # Replace with your Boost installation path
target_link_directories(PowersOfFive PUBLIC ${Boost_Multiprecision_DIR})
link_directories(PowersOfFive ${Boost_Multiprecision_DIR})
