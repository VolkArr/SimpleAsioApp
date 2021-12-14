cmake_minimum_required(VERSION 3.1)

project(client)

add_executable(${PROJECT_NAME} main.cpp)

message(${Client_DIR} " " ${StructConfigs})

target_include_directories(${PROJECT_NAME} PUBLIC  Client_DIR StructConfigs ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${BOOST_LIBRAIRES} OpenSSL::SSL pthread head)
