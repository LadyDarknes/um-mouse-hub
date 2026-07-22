cmake_minimum_required(VERSION 3.10)
project(um-mouse-hub CXX)
set(CMAKE_CXX_STANDARD 20)
file(GLOB METHOD_SRCS src/methods/*.cpp)
add_executable(um-mouse-hub src/main.cpp ${METHOD_SRCS} src/methods/imethod.hpp)
if(MSVC)
    target_link_libraries(um-mouse-hub PRIVATE setupapi hid windowsapp dinput8 dxguid)
else()
    target_link_libraries(um-mouse-hub PRIVATE setupapi hid dinput8 dxguid)
endif()


