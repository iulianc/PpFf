def:
	make -f gt-makefile

# Specify the final target name
EXE 		:= testAll

# Specify the tests path
ROOT_TESTS 	:= $(wildcard *.cpp)
# Specify the pipe path
ROOT_PIPE 	:= $(HOME)/ProjectMemoir/Release2/PpFf/src

# Get the corresponding object file list
OBJ 		:= $(ROOT_TESTS:.cpp=.o)
# From the object file list, get the dependency file list to handle automatic
DEP 		:= $(OBJ:.o=.d)

# Compiler
CXX		= g++ -std=c++17
# Specify preprocessor flags
CPPFLAGS 	:= -I./ -I $(ROOT_PIPE)
# Required flags to enable the automatic dependency generation by the compiler
CPPFLAGS 	+= -MMD -MP
# Warning flags
CXXFLAGS  	= -Wall
# Specify linker libraries
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

