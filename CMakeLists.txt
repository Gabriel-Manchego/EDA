project("BallStarTree")

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILER g++)

find_package(Eigen3 REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE Eigen3::Eigen)


