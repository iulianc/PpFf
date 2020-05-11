.IGNORE:
def_run:
	make -f gt-makefile



TEST_FILE_78792="testdata/78792Words.txt"
ATTENDUS_78792="attendus-78792.txt"

TEST_FILE_loremipsum="testdata/loremipsum.txt"
ATTENDUS_loremipsum="attendus-loremipsum.txt"

TEST_FILE_752856="testdata/752856Words.txt"
ATTENDUS_752856="attendus-752856.txt"

#MAP_TYPE=-D__ORDERED_MAP__

DEBUG=1
NB_FOIS=30
TEST_FILE_2137758="testdata/2137758Words.txt"


########################################################

# Specify the final target name
EXE 		:= WordCount WordCount.class WordCountWarmup.class WordCountFastFlow WordCountFastFlowType # WordCountSeq

# Specify the tests path
ROOT_TESTS 	:= $(wildcard *.cpp)
# Specify the pipe path
#ROOT_PIPE      := $(HOME)/RepositoryFastFlow/PpFf_Catch/src/pp
ROOT_PIPE       := $(HOME)/Memoire/src

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

# Tell make that these target are not real files
.PHONY: all clean cleanall
.SUFFIXES: .cpp 

compile: $(EXE)

run: run_cpp run_java run_fastflow run_fastflow_type test_warmup run_java_warmup #run_cpp_seq

run_cpp: compile
	@echo "*** Execution C++ *** "
	@./WordCount 1 $(TEST_FILE_loremipsum) $(DEBUG) | sort > obtenus-cpp.txt
	@diff obtenus-cpp.txt $(ATTENDUS_loremipsum)

	@./WordCount 2 $(TEST_FILE_78792) $(DEBUG) | sort > obtenus-cpp.txt
	@diff obtenus-cpp.txt $(ATTENDUS_78792)

	@./WordCount 2 $(TEST_FILE_752856) $(DEBUG) | sort > obtenus-cpp.txt
	@diff obtenus-cpp.txt $(ATTENDUS_752856)


run_java: compile
	@echo "*** Execution Java ***"
	@java -cp . WordCount $(TEST_FILE_loremipsum) $(DEBUG) | sort > obtenus-java.txt
	@diff obtenus-java.txt $(ATTENDUS_loremipsum)

	@java -cp . WordCount $(TEST_FILE_78792) $(DEBUG) | sort > obtenus-java.txt
	@diff obtenus-java.txt $(ATTENDUS_78792)

	@java -cp . WordCount $(TEST_FILE_752856) $(DEBUG) | sort > obtenus-java.txt
	@diff obtenus-java.txt $(ATTENDUS_752856)


run_cpp_seq: compile
	@echo "*** Generation de fichiers attendus C++ *** "
	@./WordCountSeq $(TEST_FILE_loremipsum) | sort > $(ATTENDUS_loremipsum)
	@./WordCountSeq $(TEST_FILE_78792) | sort > $(ATTENDUS_78792)
	@./WordCountSeq $(TEST_FILE_752856) | sort > $(ATTENDUS_752856)


run_fastflow: compile
	@echo "*** Execution FastFlow C++ *** "
	@./WordCountFastFlow 2 $(TEST_FILE_loremipsum) $(DEBUG) | sort > obtenus-fastflow.txt
	@diff obtenus-fastflow.txt $(ATTENDUS_loremipsum)

	@./WordCountFastFlow 2 $(TEST_FILE_78792) $(DEBUG) | sort > obtenus-fastflow.txt
	@diff obtenus-fastflow.txt $(ATTENDUS_78792)

	@./WordCountFastFlow 2 $(TEST_FILE_752856) $(DEBUG) | sort > obtenus-fastflow.txt
	@diff obtenus-fastflow.txt $(ATTENDUS_752856)

run_fastflow_type: compile
	@echo "*** Execution FastFlow Type C++ *** "
	@./WordCountFastFlowType 2 $(TEST_FILE_loremipsum) $(DEBUG) | sort > obtenus-fastflow.txt
	@diff obtenus-fastflow.txt $(ATTENDUS_loremipsum)

	@./WordCountFastFlowType 2 $(TEST_FILE_78792) $(DEBUG) | sort > obtenus-fastflow.txt
	@diff obtenus-fastflow.txt $(ATTENDUS_78792)

	@./WordCountFastFlowType 2 $(TEST_FILE_752856) $(DEBUG) | sort > obtenus-fastflow.txt
	@diff obtenus-fastflow.txt $(ATTENDUS_752856)

run_java_warmup: compile
	@echo "*** Execution Java avec Warmup ***"
	@java -cp . WordCountWarmup $(TEST_FILE_752856) 1 | sort > obtenus-java-warmup.txt
	diff obtenus-java-warmup.txt $(ATTENDUS_752856)


test_warmup: compile
	@echo "*** Execution Java sans Warmup ***"
	@./testerWarmup.sh $(TEST_FILE_2137758) 0 $(NB_FOIS)
	@echo "*** Execution Java avec Warmup ***"
	@./testerWarmup.sh $(TEST_FILE_2137758) 2 $(NB_FOIS) 2> /dev/null



WordCount: $(OBJ)
		  $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OPTFLAGS) WordCount.o $(LDLIBS) -o WordCount

WordCountSeq: $(OBJ)
		  $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OPTFLAGS) WordCountSeq.o $(LDLIBS) -o WordCountSeq

WordCount.class: WordCount.java
	javac WordCount.java

WordCountFastFlow: $(OBJ)
		  $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OPTFLAGS) WordCountFastFlow.o $(LDLIBS) -o WordCountFastFlow

WordCountFastFlowType: $(OBJ)
		  $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OPTFLAGS) WordCountFastFlowType.o $(LDLIBS) -o WordCountFastFlowType

WordCountWarmup.class: WordCountWarmup.java
	javac WordCountWarmup.java

clean: 
	\rm -f $(EXE) $(OBJ) $(DEP)
	\rm -f *.class

cleanall cleanxtra: clean
	\rm -f *.o *~
	\rm -f obtenus*txt temps-sh.txt res.txt

# Include the dependency
-include $(DEP)

run_bms:
	./run_bms.rb 'WordCount' | tee temps-wc.txt

