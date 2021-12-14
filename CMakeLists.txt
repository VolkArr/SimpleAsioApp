cmake_minimum_required(VERSION 3.1)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(StructConfigs ${PROJECT_SOURCE_DIR}/StructConfigs)
set(Server_DIR ${PROJECT_SOURCE_DIR}/Server)
set(Client_DIR ${PROJECT_SOURCE_DIR}/Client)


find_package(Boost ${Boost_VERSION} REQUIRED)
find_package(OpenSSL REQUIRED)



add_subdirectory(StructConfigs)
add_subdirectory(Client)
add_subdirectory(Server)
