cmake_minimum_required(VERSION 3.5)

if(NETTYPE MATCHES "client")
    set(PrjName test_client)
    message(STATUS "Build client...")
else()
    set(PrjName test_server)
endif()

project(${PrjName})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINAYR_PATH}/bin)
set(TCPPATH ${PROJECT_SOURCE_DIR}/../..)
message(STATUS "the TCPPATH is ${TCPPATH}")

add_compile_options(--std=c++11)
add_compile_options(-fPIC)
add_compile_options(-g)

if(NETTYPE MATCHES "client")
    file(GLOB cpplist "test_client.cpp" "${TCPPATH}/tcp_network/*.cpp" "${TCPPATH}/public/*.cpp")
else()
    file(GLOB cpplist "test_server.cpp" "${TCPPATH}/tcp_network/*.cpp" "${TCPPATH}/public/*.cpp")
endif()

message(STATUS "the cpp list is ${cpplist}")

#aux_source_directory(${TCPPATH}/tcp_server/ cpp1)

set (includelist ${TCPPATH}/tcp_network/
                 ${TCPPATH}/public/)

message(STATUS "the includelist list is ${includelist}")

include_directories(${includelist}) 

add_executable(${PrjName} ${cpplist})      

target_link_libraries(${PrjName} pthread)          
