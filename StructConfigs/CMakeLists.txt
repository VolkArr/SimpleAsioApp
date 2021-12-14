cmake_minimum_required(VERSION 3.1)

project(StructConfig) 

add_library(head SHARED headers.hpp)
set_target_properties(head PROPERTIES LINKER_LANGUAGE CXX LINKER_LANGUAGE C)

