cmake_minimum_required(VERSION 3.20)
project(AosVsSoa)
set(CMAKE_BUILD_TYPE "RelWithDebInfo")

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED container)

add_executable(aos
        main.cpp
        Tree.cpp
        RenderData.h
        Rectangle.h
)

target_compile_definitions(aos PRIVATE AOS=1)
target_link_libraries(aos PRIVATE Boost::container)


add_executable(soa
        main.cpp
        Tree.cpp
        RenderData.h
        Rectangle.h
)

target_compile_definitions(soa PRIVATE SOA=1)
target_link_libraries(soa PRIVATE Boost::container)
