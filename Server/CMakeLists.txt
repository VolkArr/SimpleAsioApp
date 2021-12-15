cmake_minimum_required(VERSION 3.1)

project(server)

add_executable(${PROJECT_NAME} main.cpp)


message(${Server_DIR} " " ${StructConfigs})

target_include_directories(${PROJECT_NAME} PUBLIC Server_DIR StructConfigs ${Boost_INCLUDE_DIR} )
target_link_libraries(${PROJECT_NAME} PUBLIC ${BOOST_LIBRAIRES} OpenSSL::SSL pthread  Simple_Framework)
