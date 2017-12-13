
# Specify the final target name
EXE 		:= WordCount

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

# Tell make that these target are not real files
.PHONY: all clean cleanall
.SUFFIXES: .cpp 

default: run

compile: clean $(EXE)
	javac WordCount.java

run: run_api run_java

run_api: compile
	@echo "*** Execution C++ *** "
	@./WordCount > obtenus-CPP.txt
	@diff obtenus-CPP.txt attendus.txt

run_java: compile
	@echo "*** Execution Java ***"
	@java -cp . WordCount > obtenus-java.txt
	@diff obtenus-java.txt attendus.txt

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

