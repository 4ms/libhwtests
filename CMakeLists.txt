cmake_minimum_required(VERSION 3.12)

add_library(libhwtests 
	STATIC
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
target_include_directories(libhwtests PUBLIC ${CMAKE_CURRENT_LIST_DIR}/inc)

# All users of this library will need at least C++17
target_compile_features(libhwtests PUBLIC cxx_std_17)
