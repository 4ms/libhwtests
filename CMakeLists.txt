# This creates a target named "libhwtests${target}" so that we can add_subdirectory() this dir more than once in a
# multi-build project (i.e. for different chips/architectures)

add_library(
  libhwtests${target} STATIC
  src/AdcChecker.cc
  src/AdcRangeChecker.cc
  src/ButtonChecker.cc
  src/CodecCallbacks.cc
  src/GateInChecker.cc
  src/GateOutChecker.cc
  src/GateOutput.cc
  src/LEDTester.cc
)

# We need this directory, and users of our library will need it too
target_include_directories(libhwtests${target} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc)

# All users of this library will need at least C++17
target_compile_features(libhwtests${target} PUBLIC cxx_std_17)
