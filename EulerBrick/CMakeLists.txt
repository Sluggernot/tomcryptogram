
cmake_minimum_required(VERSION 3.25)
project(EulerBrick)

set(CMAKE_CXX_STANDARD 20)

add_executable(EulerBrick
        main.cpp
)

#find_package(Eigen3 CONFIG REQUIRED)
target_link_libraries(EulerBrick
        PRIVATE
        gmp
        gmpxx
        #tommath
        #Eigen3::Eigen
)

set(Boost_USE_STATIC_RPATH ON)
find_path(Boost_Multiprecision_DIR /usr/include/boost)  # Replace with your Boost installation path
target_link_directories(EulerBrick PUBLIC ${Boost_Multiprecision_DIR})
link_directories(EulerBrick ${Boost_Multiprecision_DIR})
