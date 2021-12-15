cmake_minimum_required(VERSION 3.1)

project(Simple_Framework) 

SET (files
    headers.hpp
    message.hpp
    client_interface.hpp
    server_interface.hpp
    TcpQueue.hpp
    connection.hpp
    client_libs.hpp
    server_libs.hpp
)

add_library(${PROJECT_NAME} SHARED ${files})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX LINKER_LANGUAGE C)

