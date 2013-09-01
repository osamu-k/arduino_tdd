ifndef SILENCE
	SILENCE = @
endif

COMPONENT_NAME = test001
CPPUTEST_HOME = ~/Developer/CppUTest/cpputest

CPPUTEST_USE_EXTENSIONS = Y
CPP_PLATFORM = Gcc

#CPPUTEST_MEMLEAK_DETECTOR_NEW_MACRO_FILE = -include src/NewOverrides.h

SRC_DIRS = src
TEST_SRC_DIRS = test
INCLUDE_DIRS = . src $(CPPUTEST_HOME)/include

include $(CPPUTEST_HOME)/build/MakefileWorker.mk

