cmake_minimum_required(VERSION 3.9)
project(ex3 C)

set(CMAKE_C_STANDARD 99)
link_directories(librarys)

add_executable(ex3 main.c list.h set.h mtm_ex3.h list_mtm.h list_mtm.c test_utilities.h student.h student.c)
target_link_libraries(ex3 libmtm.a)
set(CMAKE_C_FLAGS "-std=c99 -Wall -Werror -pedantic-errors")
