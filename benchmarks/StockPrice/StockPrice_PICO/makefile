
# Specify the final target name
EXE 		:= StockPrice

# Specify the tests path
ROOT_TESTS 	:= $(wildcard *.cpp)

# Get the corresponding object file list
OBJ 		:= $(ROOT_TESTS:.cpp=.o)
# From the object file list, get the dependency file list to handle automatic
DEP 		:= $(OBJ:.o=.d)

# Compiler
CXX		= g++ -std=c++11 $(MAP_TYPE)
# Required flags to enable the automatic dependency generation by the compiler
CPPFLAGS 	+= -MMD -MP
# Warning flags
CXXFLAGS  	= -Wall
# Specify linker libraries
LDLIBS 		:= -lpthread

OPTFLAGS	=  -O3 -finline-functions -DNDEBUG

TEST_FILE="../testdata/stock_options_64K.txt"
OUT_FILE="../testdata/pico_result.txt"

# Tell make that these target are not real files
.PHONY: all clean cleanall
.SUFFIXES: .cpp 

default: run

compile: $(EXE)

run: run_cpp

run_cpp: compile
	@echo "*** Execution C++ *** "
	@./StockPrice $(TEST_FILE) $(OUT_FILE)


all		: $(EXE)
	./StockPrice 


$(EXE)		: $(OBJ)
		  $(CXX) $(CXXFLAGS) $(OPTFLAGS) $^ $(LDLIBS) -o $@

clean: 
	\rm -f $(EXE) $(OBJ) $(DEP)
	\rm -f *.class

cleanall cleanxtra: clean
	\rm -f *.o *~
	\rm -f obtenus*txt

# Include the dependency
-include $(DEP)


