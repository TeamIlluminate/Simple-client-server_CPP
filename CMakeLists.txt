cmake_minimum_required(VERSION 3.0.0)
project(ISPT VERSION 0.1.0)

include(CTest)
enable_testing()

aux_source_directory(src SRC)
include_directories(src)

add_executable(ISPT src/main.cpp ${SRC})
target_link_libraries(ISPT -lpthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
