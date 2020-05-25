#Set this to @ to keep the makefile quiet
# SILENCE = @

#---- Outputs ----#
COMPONENT_NAME = localtests

#--- Inputs ----#
CPPUTEST_HOME = tests/cpputest
CPP_PLATFORM = Gcc
CC = gcc
PROJECT_HOME_DIR = .

SRC_FILES = \
	src/AdcRangeChecker.cc \
	src/LEDButtonChecker.cc

# SRC_DIRS = \
# 	src \

TEST_SRC_DIRS = \
	tests \
	tests/stubs \
	tests/stubs/driver \

INCLUDE_DIRS =\
  $(CPPUTEST_HOME)/include\
  tests/fff \
  tests/ \
  tests/stubs \
  tests/stubs/driver \
  inc \

MOCKS_SRC_DIRS = \
	mocks \
	mocks/drivers \

CPPUTEST_WARNINGFLAGS += -Wall 
CPPUTEST_WARNINGFLAGS += -Werror
CPPUTEST_WARNINGFLAGS += -Wswitch-default
CPPUTEST_WARNINGFLAGS += -Wno-format-nonliteral
CPPUTEST_WARNINGFLAGS += -Wno-sign-conversion 
CPPUTEST_WARNINGFLAGS += -Wno-pedantic 
CPPUTEST_WARNINGFLAGS += -Wno-shadow
CPPUTEST_WARNINGFLAGS += -Wno-missing-field-initializers
CPPUTEST_WARNINGFLAGS += -Wno-unused-parameter
CPPUTEST_WARNINGFLAGS += -Wno-register
CPPUTEST_WARNINGFLAGS += -Wno-missing-noreturn

CPPUTEST_CFLAGS += -Wall
CPPUTEST_CFLAGS += -Wstrict-prototypes
CPPUTEST_CFLAGS += -pedantic
CPPUTEST_CFLAGS += -Wno-missing-prototypes
CPPUTEST_CFLAGS += -Wno-documentation\
					-Wno-documentation-unknown-command \
					-DTESTPROJECT \


CPPUTEST_CXXFLAGS += --std=c++17
CPPUTEST_CXXFLAGS += -Wno-c++98-compat-pedantic
CPPUTEST_CXXFLAGS += -Wno-c++98-compat
CPPUTEST_CXXFLAGS += -Wno-inconsistent-missing-destructor-override
CPPUTEST_CXXFLAGS += -Wno-documentation \
					-Wno-documentation-unknown-command\
					-Wno-missing-noreturn \
					-DTESTPROJECT \

#LD_LIBRARIES = -lpthread

-include $(CPPUTEST_HOME)/build/MakefileWorker.mk

.PHONY: cpputest
cpputest: cpputest/lib/libCppUTest.a

cpputest/lib/libCppUTest.a:
	cd tests/cpputest; autoreconf . -i && ./configure --enable-std-cpp14 && make tdd && cd ../..

