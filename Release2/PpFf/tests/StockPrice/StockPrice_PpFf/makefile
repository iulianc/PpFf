
# Oter le commentaire de la 2e ligne pour desactiver JIT de Java
AVEC_SANS_JIT=-Djava.compiler=NONE
#AVEC_SANS_JIT=

def: _exemple_jit_

_exemple_jit_: 
	make compile
	@java -cp . StockPrice $(TEST_FILE) 1 | sort > obtenus-java.txt
	@java $(AVEC_SANS_JIT) -cp . StockPrice $(TEST_FILE) 1 | sort > obtenus-java.txt
	@java -cp . StockPrice $(TEST_FILE) 10 | sort > obtenus-java.txt
	@java $(AVEC_SANS_JIT) -cp . StockPrice $(TEST_FILE) 10 | sort > obtenus-java.txt

benchmarks: compile
	@echo "*** Execution avec divers nombres d'iterations et de threads *** "
	echo "" > $(OUTPUT_FILE_RESULT)
	@./StockPrice $(TEST_FILE) 1 1 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 1 2 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 1 3 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 1 4 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 1 8 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 1 16 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 1 32 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . StockPrice $(TEST_FILE) 1 $(OUTPUT_FILE_RESULT) | sort > obtenus-java.txt
	@echo
	@./StockPrice $(TEST_FILE) 2 1 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 2 2 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 2 3 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 2 4 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 2 8 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 2 16 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 2 32 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . StockPrice $(TEST_FILE) 2 $(OUTPUT_FILE_RESULT) | sort > obtenus-java.txt
	@echo
	@./StockPrice $(TEST_FILE) 4 1 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 4 2 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 4 3 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 4 4 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 4 8 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 4 16 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 4 32 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . StockPrice $(TEST_FILE) 4 $(OUTPUT_FILE_RESULT) | sort > obtenus-java.txt
	@echo
	@./StockPrice $(TEST_FILE) 8 1 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 8 2 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 8 3 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 8 4 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 8 8 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 8 16 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 8 32 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . StockPrice $(TEST_FILE) 8 $(OUTPUT_FILE_RESULT) | sort > obtenus-java.txt
	@echo
	@./StockPrice $(TEST_FILE) 16 1 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 16 2 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 16 3 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 16 4 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 16 8 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 16 16 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 16 32 $(OUTPUT_FILE_RESULT) | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . StockPrice $(TEST_FILE) 16 $(OUTPUT_FILE_RESULT) | sort > obtenus-java.txt

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

run:  run_cpp run_java
default: run

compile: $(EXE)

run_cpp: compile
	@echo "*** Execution C++ *** "
	@./StockPrice $(TEST_FILE) $(NB_TIMES) | sort > obtenus-cpp.txt
	@diff obtenus-cpp.txt $(ATTENDUS)


run_java: compile
	@echo "*** Execution Java ***"
	@java $(AVEC_SANS_JIT) -cp . StockPrice $(TEST_FILE) $(NB_TIMES) | sort > obtenus-java.txt
	@diff obtenus-java.txt $(ATTENDUS)

all		: $(EXE)
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


