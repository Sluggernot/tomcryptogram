cmake_minimum_required(VERSION 3.25)
project(WeirdNumbers)

set(CMAKE_CXX_STANDARD 20)

add_executable(WeirdNumbers
        main.cpp
)

#find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(WeirdNumbers
        PRIVATE
        gmp
        gmpxx
        #tommath
        #Eigen3::Eigen
)

set(Boost_USE_STATIC_RPATH ON)
find_path(Boost_Multiprecision_DIR /usr/include/boost)  # Replace with your Boost installation path
target_link_directories(WeirdNumbers PUBLIC ${Boost_Multiprecision_DIR})
link_directories(WeirdNumbers ${Boost_Multiprecision_DIR})
