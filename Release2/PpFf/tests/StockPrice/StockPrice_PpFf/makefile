_def: compile
	@java -cp . StockPrice $(TEST_FILE) 1 | sort > obtenus-java.txt
	@java -cp . StockPrice $(TEST_FILE) 2 | sort > obtenus-java.txt
	@java -cp . StockPrice $(TEST_FILE) 4 | sort > obtenus-java.txt
	@java -cp . StockPrice $(TEST_FILE) 8 | sort > obtenus-java.txt
	@java -cp . StockPrice $(TEST_FILE) 16 | sort > obtenus-java.txt
	@java -cp . StockPrice $(TEST_FILE) 32 | sort > obtenus-java.txt
	@java -cp . StockPrice $(TEST_FILE) 64 | sort > obtenus-java.txt
	@java -cp . StockPrice $(TEST_FILE) 128 | sort > obtenus-java.txt

def: compile
	@echo "*** Execution avec divers nombres d'iterations et de threads *** "
	@./StockPrice $(TEST_FILE) 1 1 | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 1 2 | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 1 3 | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 1 4 | sort > obtenus-cpp.txt
	@java -cp . StockPrice $(TEST_FILE) 1 | sort > obtenus-java.txt
	@echo
	@./StockPrice $(TEST_FILE) 2 1 | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 2 2 | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 2 3 | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 2 4 | sort > obtenus-cpp.txt
	@java -cp . StockPrice $(TEST_FILE) 2 | sort > obtenus-java.txt
	@echo
	@./StockPrice $(TEST_FILE) 4 1 | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 4 2 | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 4 3 | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 4 4 | sort > obtenus-cpp.txt
	@java -cp . StockPrice $(TEST_FILE) 4 | sort > obtenus-java.txt
	@echo
	@./StockPrice $(TEST_FILE) 8 1 | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 8 2 | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 8 3 | sort > obtenus-cpp.txt
	@./StockPrice $(TEST_FILE) 8 4 | sort > obtenus-cpp.txt
	@java -cp . StockPrice $(TEST_FILE) 8 | sort > obtenus-java.txt

.IGNORE:

NB_TIMES=5

# Specify the final target name
EXE 		:= StockPrice

# Specify the tests path
ROOT_TESTS 	:= $(wildcard *.cpp)
# Specify the pipe path
#ROOT_PIPE      := $(HOME)/PpFf_JapetServer/src
ROOT_PIPE      := $(HOME)/PpFf/Release2/PpFf/src

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
	@java -cp . StockPrice $(TEST_FILE) $(NB_TIMES) | sort > obtenus-java.txt
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


