cmake_minimum_required(VERSION 3.28)
project(MagicSquares)

set(CMAKE_CXX_STANDARD 20)

add_executable(MagicSquares
        main.cpp
        MagicSquare_data.h
        MagicSquare_data.cpp
        squares_container.h
        squares_container.cpp
)

find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(MagicSquares
        PRIVATE
        gmp
        gmpxx
        #tommath
        #Eigen3::Eigen
)

set(Boost_USE_STATIC_RPATH ON)
find_path(Boost_Multiprecision_DIR /usr/include/boost)  # Replace with your Boost installation path
target_link_directories(MagicSquares PUBLIC ${Boost_Multiprecision_DIR})
link_directories(MagicSquares ${Boost_Multiprecision_DIR})
