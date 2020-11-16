cmake_minimum_required(VERSION 3.10)

project(simple_web_server VERSION 1.0)

add_executable(${CMAKE_PROJECT_NAME}
	src/main.cpp
	src/web_server.cpp
)

include_directories(include)