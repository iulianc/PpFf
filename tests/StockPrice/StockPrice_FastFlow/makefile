#MAP_TYPE=-D__ORDERED_MAP__
NB_TIMES=1

# Specify the final target name
EXE 		:= StockPriceFastFlow

# Specify the tests path
ROOT_TESTS 	:= $(wildcard *.cpp)
# Specify the pipe path
ROOT_PIPE      := $(HOME)/ProjectMemoir/Release2/PpFf/src
#ROOT_PIPE 	:= $(HOME)/PpFf/src/pp

# Get the corresponding object file list
OBJ 		:= $(ROOT_TESTS:.cpp=.o)
# From the object file list, get the dependency file list to handle automatic
DEP 		:= $(OBJ:.o=.d)

# Compiler
CXX		= g++ -std=c++11 $(MAP_TYPE)
# Specify preprocessor flags
CPPFLAGS 	:= -I./ -I $(ROOT_PIPE)
# Required flags to enable the automatic dependency generation by the compiler
CPPFLAGS 	+= -MMD -MP
# Warning flags
CXXFLAGS  	= -Wall
# Specify linker libraries
LDLIBS 		:= -lpthread

OPTFLAGS	=  -O3 -finline-functions -DNDEBUG

TEST_FILE="../testdata/stock_options_64K.txt"
ATTENDUS="../testdata/pico_result.txt"


# Tell make that these target are not real files
.PHONY: all clean cleanall
.SUFFIXES: .cpp 

default: run

compile: $(EXE)

run: run_cpp

run_cpp: compile
	@echo "*** Execution C++ *** "
	@./StockPriceFastFlow $(TEST_FILE) $(NB_TIMES) | sort > obtenus-cpp.txt
	@diff obtenus-cpp.txt $(ATTENDUS)

benchmarks: compile
	@echo "*** Execution avec divers nombres d'iterations et de threads *** "
	@./StockPriceFastFlow $(TEST_FILE) 1 1 | sort > obtenus-cpp.txt
	@./StockPriceFastFlow $(TEST_FILE) 1 2 | sort > obtenus-cpp.txt
	@./StockPriceFastFlow $(TEST_FILE) 1 3 | sort > obtenus-cpp.txt
	@./StockPriceFastFlow $(TEST_FILE) 1 4 | sort > obtenus-cpp.txt
	@echo
	@./StockPriceFastFlow $(TEST_FILE) 2 1 | sort > obtenus-cpp.txt
	@./StockPriceFastFlow $(TEST_FILE) 2 2 | sort > obtenus-cpp.txt
	@./StockPriceFastFlow $(TEST_FILE) 2 3 | sort > obtenus-cpp.txt
	@./StockPriceFastFlow $(TEST_FILE) 2 4 | sort > obtenus-cpp.txt
	@echo
	@./StockPriceFastFlow $(TEST_FILE) 4 1 | sort > obtenus-cpp.txt
	@./StockPriceFastFlow $(TEST_FILE) 4 2 | sort > obtenus-cpp.txt
	@./StockPriceFastFlow $(TEST_FILE) 4 3 | sort > obtenus-cpp.txt
	@./StockPriceFastFlow $(TEST_FILE) 4 4 | sort > obtenus-cpp.txt
	@echo
	@./StockPriceFastFlow $(TEST_FILE) 8 1 | sort > obtenus-cpp.txt
	@./StockPriceFastFlow $(TEST_FILE) 8 2 | sort > obtenus-cpp.txt
	@./StockPriceFastFlow $(TEST_FILE) 8 3 | sort > obtenus-cpp.txt
	@./StockPriceFastFlow $(TEST_FILE) 8 4 | sort > obtenus-cpp.txt
	@echo
	#@./StockPriceFastFlow $(TEST_FILE) 16 1 | sort > obtenus-cpp.txt
	#@./StockPriceFastFlow $(TEST_FILE) 16 2 | sort > obtenus-cpp.txt
	#@./StockPriceFastFlow $(TEST_FILE) 16 3 | sort > obtenus-cpp.txt
	#@./StockPriceFastFlow $(TEST_FILE) 16 4 | sort > obtenus-cpp.txt
	
	#@diff obtenus-cpp.txt $(ATTENDUS)

all		: $(EXE)
	./StockPriceFastFlow


$(EXE)		: $(OBJ)
		  $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OPTFLAGS) $^ $(LDLIBS) -o $@

clean: 
	\rm -f $(EXE) $(OBJ) $(DEP)
	\rm -f *.class

cleanall cleanxtra: clean
	\rm -f *.o *~
	\rm -f obtenus*txt

# Include the dependency
-include $(DEP)

