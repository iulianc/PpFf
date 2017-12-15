
# Specify the final target name
EXE 		:= WordCount WordCount.class

# Specify the tests path
ROOT_TESTS 	:= $(wildcard *.cpp)
# Specify the pipe path
ROOT_PIPE 	:= $(HOME)/RepositoryFastFlow/PpFf_Catch/src/pp

# Get the corresponding object file list
OBJ 		:= $(ROOT_TESTS:.cpp=.o)
# From the object file list, get the dependency file list to handle automatic
DEP 		:= $(OBJ:.o=.d)

# Compiler
CXX		= g++ -std=c++11
# Specify preprocessor flags
CPPFLAGS 	:= -I./ -I $(ROOT_PIPE)
# Required flags to enable the automatic dependency generation by the compiler
CPPFLAGS 	+= -MMD -MP
# Warning flags
CXXFLAGS  	= -Wall
# Specify linker libraries
LDLIBS 		:= -lpthread

OPTFLAGS	=  -O3 -finline-functions -DNDEBUG

TEST_FILE="testdata/78792Words.txt"
ATTENDUS="attendus-78792.txt"

#TEST_FILE="testdata/loremipsum.txt"
#ATTENDUS="attendus-loremipsum.txt"

#TEST_FILE="testdata/1505712Words.txt"
#ATTENDUS="attendus-1505712.txt"


# Tell make that these target are not real files
.PHONY: all clean cleanall
.SUFFIXES: .cpp 

default: run

compile: $(EXE)
	javac WordCount.java

run: run_cpp run_java

run_cpp: compile
	@echo "*** Execution C++ *** "
	@./WordCount $(TEST_FILE) | sort > obtenus-cpp.txt
	@diff obtenus-cpp.txt $(ATTENDUS)

run_java: compile
	@echo "*** Execution Java ***"
	@java -cp . WordCount $(TEST_FILE) | sort > obtenus-java.txt
	@diff obtenus-java.txt $(ATTENDUS)

all		: $(EXE)
	./WordCount


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

