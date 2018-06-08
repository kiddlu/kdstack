cmake_minimum_required(VERSION 2.8)
project (iocached C)
add_definitions (-Wall -Werror)

#static link
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/*.c")
add_executable(pioprio  ${SOURCES})
