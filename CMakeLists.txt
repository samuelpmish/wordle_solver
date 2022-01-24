cmake_minimum_required(VERSION 3.15)
project(wordle LANGUAGES CXX)
add_executable(wordle_solver main.cpp word_list.cpp)
target_compile_features(wordle_solver PUBLIC cxx_std_17)
