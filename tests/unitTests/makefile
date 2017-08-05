
# Specify the final target name
EXE 		:= testAll

# Specify the tests path
ROOT_TESTS 	:= $(wildcard *.cpp)
# Specify the pipe path
ROOT_PIPE 	:= $(HOME)/RepositoryFastFlow/PpFf/src/pp
# Specify the google tests lib path
#ROOT_GTEST	:= $(HOME)/RepositoryFastFlow/PpFf/tests/googleTestLib/gtest
#ROOT_GTEST	:= /opt/gtest/lib
ROOT_GTEST	:= $(HOME)/RepositoryFastFlow/PpFf/tests/googleTestLib
ROOT_GTEST_LIB	:= $(HOME)/RepositoryFastFlow/PpFf/tests/googleTestLib/gtest


SRC_GTEST := $(wildcard $(ROOT_GTEST_LIB)/*.cc)
#HEADERSRC := $(wildcard $(ROOT_GTEST)/*.h)


# Get the corresponding object file list
OBJ 		:= $(ROOT_TESTS:.cpp=.o) $(SRC_GTEST:.cc=.o)
# From the object file list, get the dependency file list to handle automatic
DEP 		:= $(OBJ:.o=.d)

# Compiler
CXX		= g++ -std=c++11
# Specify preprocessor flags
CPPFLAGS 	:= -I./ -I $(ROOT_PIPE) -I$(ROOT_GTEST)
# Required flags to enable the automatic dependency generation by the compiler
CPPFLAGS 	+= -MMD -MP
# Warning flags
CXXFLAGS  	= -Wall
# Specify linker libraries
#LDLIBS 	:= $(ROOT_GTEST)/libgtest.a $(ROOT_GTEST)/libgtest_main.a -lpthread
LDLIBS 		:= -lpthread

OPTFLAGS	=  -O3 -finline-functions -DNDEBUG

# Tell make that these target are not real files
.PHONY: all clean cleanall
.SUFFIXES: .cpp 

all		: $(EXE)

$(EXE)		: $(OBJ)
		  $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OPTFLAGS) $^ $(LDLIBS) -o $@

clean		: rm -f $(EXE) $(OBJ) $(DEP)
cleanall	: clean
		  \rm -f *.o *~

# Include the dependency
-include $(DEP)

