cmake_minimum_required(VERSION 3.0)

set(PrjName TcpTestServer)
project(${PrjName})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINAYR_PATH}/bin)
set(TCPPATH ${PROJECT_SOURCE_DIR}/..)
message(STATUS "the TCPPATH is ${TCPPATH}")

add_compile_options(--std=c++11)
add_compile_options(-fPIC)
add_compile_options(-g)

file(GLOB cpplist "*.cpp" "${TCPPATH}/tcp_server/*.cpp" "${TCPPATH}/public/*.cpp")
message(STATUS "the cpp list is ${cpplist}")

#aux_source_directory(${TCPPATH}/tcp_server/ cpp1)

set (includelist ${TCPPATH}/tcp_server/
                 ${TCPPATH}/public/)

message(STATUS "the includelist list is ${includelist}")

include_directories(${includelist}) 

add_executable(${PrjName} ${cpplist})      

target_link_libraries(${PrjName} pthread)          
