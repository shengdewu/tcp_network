cmake_minimum_required(VERSION 3.0)

set(PrjName log_test)
project(${PrjName})

add_compile_options(--std=c++11)
add_compile_options(-fPIC)
add_compile_options(-g)
add_compile_options(-Wall)

file(GLOB cpplist "*.cpp")
file(GLOB_RECURSE  REMOVE_FILE "build/*")
list(REMOVE_ITEM cpplist ${REMOVE_FILE})


include_directories(../../) 

add_executable(${PrjName} ${cpplist})      

target_link_libraries(${PrjName} pthread)          
