cmake_minimum_required(VERSION 3.21.2)

project(libayaztub VERSION 0.1.0 LANGUAGES C)
add_library(libayaztub SHARED "")
set_target_properties(libayaztub
  PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
    OUTPUT_NAME ayaztub)
set(HEADERS include/ayaztub.h)

target_include_directories(libayaztub
  PUBLIC
    "include/")
target_include_directories(libayaztub
  PRIVATE
    "src/")

target_compile_options(libayaztub
  PRIVATE
    -Wall -Wextra -Werror -pedantic -Wvla --std=c99)
add_subdirectory(src)

install(TARGETS libayaztub
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
