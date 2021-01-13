cmake_minimum_required(VERSION 3.10)

project(simple_web_server VERSION 1.0)

find_package (Threads)

add_executable(${CMAKE_PROJECT_NAME}
	src/main.cpp
	src/web_server.cpp
	src/thread_pool.cpp
)

include_directories(include)

target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
