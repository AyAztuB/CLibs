# Criterion-based test binary setup
find_library(CRITERION NAMES criterion libcriterion REQUIRED)

add_custom_target(tests)

# function definition to create and link test executable with the ctest cmd
function(package_add_test TESTNAME)
  # Add the test executable with its sources
  add_executable(${TESTNAME} ${ARGN})

  # Link the test executable with criterion
  target_link_libraries(${TESTNAME}
    PRIVATE
      ${CRITERION})
  # Add the private include in the test includes
  target_include_directories(${TESTNAME}
    PRIVATE
      ${CMAKE_SOURCE_DIR}/include
      ${CMAKE_SOURCE_DIR}/test
      ${CMAKE_SOURCE_DIR}/src)

  # Link the test with gcov and compile it with coverage to test its coverage
  target_link_libraries(${TESTNAME}
    PRIVATE
      gcov pthread)
  target_compile_options(${TESTNAME}
    PRIVATE
      --coverage -fPIC -g -O0)

  # Add the test in the ctest command
  add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})

  add_dependencies(tests ${TESTNAME})
endfunction()

package_add_test(logger_test
  logger_tests.c
  ${CMAKE_SOURCE_DIR}/src/Logger/logger.c)
