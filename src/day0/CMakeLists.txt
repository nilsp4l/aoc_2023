cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)


set(EXECUTABLE_NAME run_challenge)



add_executable(${EXECUTABLE_NAME} day0.cpp)



target_link_libraries(${EXECUTABLE_NAME} PUBLIC utility)