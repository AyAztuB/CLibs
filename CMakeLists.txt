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
    -Wall -Wextra -Werror -pedantic -Wvla --std=c99 -Wno-attributes)

target_link_libraries(libayaztub PUBLIC pthread)

add_subdirectory(src)

install(TARGETS libayaztub
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)

# Enable testing
enable_testing()

# Criterion-based test binary setup
find_library(CRITERION NAMES criterion libcriterion REQUIRED)

# Add the test executable
add_executable(tests "")

# Link the library and Criterion to the test executable
target_link_libraries(tests PRIVATE libayaztub ${CRITERION})

# Add test directory to include paths for tests
target_include_directories(tests PRIVATE include test src)

# Add the test executable to ctest
add_test(NAME ayaztub_tests COMMAND tests)

add_subdirectory(test)
