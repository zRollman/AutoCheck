cmake_minimum_required (VERSION 3.5.1)
project (demo)
set(CMAKE_C_FLAGS "${CMAKE_CFLAGS} -Wall -O3 -mcmodel=medium")

LINK_LIBRARIES(m)
aux_source_directory (../../libcheckpoint CPK_LIST)
aux_source_directory (src SRC_LIST)
set(cpk_directory "build/cpk_directory")
file(MAKE_DIRECTORY ${cpk_directory})
file(COPY ../delete.sh DESTINATION ./)

SET(CMAKE_C_COMPILER "/usr/bin/gcc")
SET(CMAKE_CXX_COMPILER "/usr/bin/g++")
add_executable (run  ${SRC_LIST}  ${CPK_LIST} )
