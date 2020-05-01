
# Oter le commentaire de la 2e ligne pour desactiver JIT de Java
AVEC_SANS_JIT=-Djava.compiler=NONE
#AVEC_SANS_JIT=

def:
	make -f gt-makefile run_bms


benchmarks: compile
	@echo "*** Execution avec divers nombres d'iterations et de threads *** "
	@./StockPrice $(TEST_FILE) 1
	@./StockPrice $(TEST_FILE) 2
	@./StockPrice $(TEST_FILE) 3
	@./StockPrice $(TEST_FILE) 4
	@./StockPrice $(TEST_FILE) 8
	@./StockPrice $(TEST_FILE) 16
	#@./StockPrice $(TEST_FILE) 32
	@java $(AVEC_SANS_JIT) -cp . StockPrice $(TEST_FILE)


.IGNORE:

NB_TIMES=5

# Specify the final target name
EXE 		:= StockPrice

# Specify the tests path
ROOT_TESTS 	:= $(wildcard *.cpp)
# Specify the pipe path
ROOT_PIPE      := $(HOME)/ProjectMemoir/Release2/PpFf/src
#ROOT_PIPE      := $(HOME)/PpFf/Release2/PpFf/src

# Get the corresponding object file list
OBJ 		:= $(ROOT_TESTS:.cpp=.o)
# From the object file list, get the dependency file list to handle automatic
DEP 		:= $(OBJ:.o=.d)

# Compiler
CXX		= g++ -std=c++17 $(MAP_TYPE)
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
OUTPUT_FILE_RESULT="../testdata/stock_benchmark_result.txt"

# Tell make that these target are not real files
.PHONY: all clean cleanall
.SUFFIXES: .cpp 


compile: $(EXE)
	javac OptionData.java StockPrice.java

compile: $(EXE)


all	: $(EXE)
	./StockPrice 


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


