cmake_minimum_required(VERSION 3.15)
project(wordle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(wordle_tools wordle_tools.cpp wordle_words.cpp wordle_answers.cpp)

add_executable(wordle_solver solver.cpp)
target_link_libraries(wordle_solver PUBLIC wordle_tools)

add_executable(wordle game.cpp)
target_link_libraries(wordle PUBLIC wordle_tools)

add_executable(wordle_analysis analysis.cpp)
target_link_libraries(wordle_analysis PUBLIC wordle_tools)
