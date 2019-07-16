#def:
#	make -f gt-makefile

# Oter le commentaire de la 2e ligne pour desactiver JIT de Java
#AVEC_SANS_JIT=-Djava.compiler=NONE
AVEC_SANS_JIT=

NB_TIMES=1

# Specify the final target name
EXE 		:= WordCount

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

OUTPUT_FILE_RESULT="testdata/wordcount_benchmark_result.txt"

WORDS_78792="testdata/78792Words.txt"
WORDS_167941="testdata/167941Words.txt"
WORDS_281307="testdata/281307Words.txt"
WORDS_482636="testdata/482636Words.txt"
WORDS_752856="testdata/752856Words.txt"
WORDS_2614743="testdata/2614743Words.txt"
WORDS_5247678="testdata/5247678Words.txt"


TEST_FILE="testdata/78792Words.txt"
ATTENDUS="attendus-78792.txt"

#TEST_FILE="testdata/loremipsum.txt"
#ATTENDUS="attendus-loremipsum.txt"

#TEST_FILE="testdata/1505712Words.txt"
#TEST_FILE="testdata/5247678Words.txt"
#ATTENDUS="attendus-1505712.txt"
#ATTENDUS="attendus-5247678.txt"


# Tell make that these target are not real files
.PHONY: all clean cleanall
.SUFFIXES: .cpp 


benchmarks: compile
	@echo "*** Execution avec divers nombres d'iterations et de threads *** "
	echo "" > $(OUTPUT_FILE_RESULT)
	@./WordCount $(WORDS_78792) 1 1 $(OUTPUT_FILE_RESULT)  78792 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_78792) 1 2 $(OUTPUT_FILE_RESULT)  78792 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_78792) 1 3 $(OUTPUT_FILE_RESULT)  78792 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_78792) 1 4 $(OUTPUT_FILE_RESULT)  78792 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_78792) 1 8 $(OUTPUT_FILE_RESULT)  78792 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_78792) 1 $(OUTPUT_FILE_RESULT) 78792 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_78792) 2 1 $(OUTPUT_FILE_RESULT)  78792 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_78792) 2 2 $(OUTPUT_FILE_RESULT)  78792 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_78792) 2 3 $(OUTPUT_FILE_RESULT)  78792 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_78792) 2 4 $(OUTPUT_FILE_RESULT)  78792 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_78792) 2 8 $(OUTPUT_FILE_RESULT)  78792 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_78792) 2 $(OUTPUT_FILE_RESULT) 78792 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_78792) 4 1 $(OUTPUT_FILE_RESULT)  78792 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_78792) 4 2 $(OUTPUT_FILE_RESULT)  78792 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_78792) 4 3 $(OUTPUT_FILE_RESULT)  78792 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_78792) 4 4 $(OUTPUT_FILE_RESULT)  78792 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_78792) 4 8 $(OUTPUT_FILE_RESULT)  78792 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_78792) 4 $(OUTPUT_FILE_RESULT) 78792 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_167941) 1 1 $(OUTPUT_FILE_RESULT) 167941 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_167941) 1 2 $(OUTPUT_FILE_RESULT) 167941 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_167941) 1 4 $(OUTPUT_FILE_RESULT) 167941 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_167941) 1 8 $(OUTPUT_FILE_RESULT) 167941 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_167941) 1 $(OUTPUT_FILE_RESULT) 167941 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_167941) 2 1 $(OUTPUT_FILE_RESULT) 167941 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_167941) 2 2 $(OUTPUT_FILE_RESULT) 167941 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_167941) 2 4 $(OUTPUT_FILE_RESULT) 167941 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_167941) 2 8 $(OUTPUT_FILE_RESULT) 167941 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_167941) 2 $(OUTPUT_FILE_RESULT) 167941 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_167941) 4 1 $(OUTPUT_FILE_RESULT) 167941 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_167941) 4 2 $(OUTPUT_FILE_RESULT) 167941 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_167941) 4 4 $(OUTPUT_FILE_RESULT) 167941 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_167941) 4 8 $(OUTPUT_FILE_RESULT) 167941 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_167941) 4 $(OUTPUT_FILE_RESULT) 167941 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_281307) 1 1 $(OUTPUT_FILE_RESULT) 281307 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_281307) 1 2 $(OUTPUT_FILE_RESULT) 281307 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_281307) 1 4 $(OUTPUT_FILE_RESULT) 281307 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_281307) 1 8 $(OUTPUT_FILE_RESULT) 281307 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_281307) 1 $(OUTPUT_FILE_RESULT) 281307 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_281307) 2 1 $(OUTPUT_FILE_RESULT) 281307 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_281307) 2 2 $(OUTPUT_FILE_RESULT) 281307 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_281307) 2 4 $(OUTPUT_FILE_RESULT) 281307 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_281307) 2 8 $(OUTPUT_FILE_RESULT) 281307 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_281307) 2 $(OUTPUT_FILE_RESULT) 281307 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_281307) 4 1 $(OUTPUT_FILE_RESULT) 281307 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_281307) 4 2 $(OUTPUT_FILE_RESULT) 281307 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_281307) 4 4 $(OUTPUT_FILE_RESULT) 281307 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_281307) 4 8 $(OUTPUT_FILE_RESULT) 281307 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_281307) 4 $(OUTPUT_FILE_RESULT) 281307 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_482636) 1 1 $(OUTPUT_FILE_RESULT) 482636 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_482636) 1 2 $(OUTPUT_FILE_RESULT) 482636 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_482636) 1 4 $(OUTPUT_FILE_RESULT) 482636 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_482636) 1 8 $(OUTPUT_FILE_RESULT) 482636 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_482636) 1 $(OUTPUT_FILE_RESULT) 482636 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_482636) 2 1 $(OUTPUT_FILE_RESULT) 482636 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_482636) 2 $(OUTPUT_FILE_RESULT) 482636 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_482636) 4 1 $(OUTPUT_FILE_RESULT) 482636 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_482636) 4 2 $(OUTPUT_FILE_RESULT) 482636 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_482636) 4 4 $(OUTPUT_FILE_RESULT) 482636 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_482636) 4 8 $(OUTPUT_FILE_RESULT) 482636 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_482636) 4 $(OUTPUT_FILE_RESULT) 482636 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_752856) 1 1 $(OUTPUT_FILE_RESULT) 752856 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_752856) 1 2 $(OUTPUT_FILE_RESULT) 752856 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_752856) 1 4 $(OUTPUT_FILE_RESULT) 752856 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_752856) 1 8 $(OUTPUT_FILE_RESULT) 752856 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_752856) 1 $(OUTPUT_FILE_RESULT) 752856 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_752856) 2 1 $(OUTPUT_FILE_RESULT) 752856 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_752856) 2 $(OUTPUT_FILE_RESULT) 752856 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_752856) 4 1 $(OUTPUT_FILE_RESULT) 752856 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_752856) 4 2 $(OUTPUT_FILE_RESULT) 752856 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_752856) 4 4 $(OUTPUT_FILE_RESULT) 752856 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_752856) 4 8 $(OUTPUT_FILE_RESULT) 752856 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_752856) 4 $(OUTPUT_FILE_RESULT) 752856 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_2614743) 1 1 $(OUTPUT_FILE_RESULT) 2614743 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_2614743) 1 2 $(OUTPUT_FILE_RESULT) 2614743 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_2614743) 1 4 $(OUTPUT_FILE_RESULT) 2614743 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_2614743) 1 8 $(OUTPUT_FILE_RESULT) 2614743 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_2614743) 1 $(OUTPUT_FILE_RESULT) 2614743 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_2614743) 2 1 $(OUTPUT_FILE_RESULT) 2614743 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_2614743) 2 $(OUTPUT_FILE_RESULT) 2614743 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_2614743) 4 1 $(OUTPUT_FILE_RESULT) 2614743 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_2614743) 4 2 $(OUTPUT_FILE_RESULT) 2614743 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_2614743) 4 4 $(OUTPUT_FILE_RESULT) 2614743 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_2614743) 4 8 $(OUTPUT_FILE_RESULT) 2614743 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_2614743) 4 $(OUTPUT_FILE_RESULT) 2614743 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_5247678) 1 1 $(OUTPUT_FILE_RESULT) 5247678 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_5247678) 1 2 $(OUTPUT_FILE_RESULT) 5247678 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_5247678) 1 4 $(OUTPUT_FILE_RESULT) 5247678 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_5247678) 1 8 $(OUTPUT_FILE_RESULT) 5247678 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_5247678) 1 $(OUTPUT_FILE_RESULT) 5247678 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_5247678) 2 1 $(OUTPUT_FILE_RESULT) 5247678 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_5247678) 2 $(OUTPUT_FILE_RESULT) 5247678 | sort > obtenus-java.txt
	@echo
	@./WordCount $(WORDS_5247678) 4 1 $(OUTPUT_FILE_RESULT) 5247678 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_5247678) 4 2 $(OUTPUT_FILE_RESULT) 5247678 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_5247678) 4 4 $(OUTPUT_FILE_RESULT) 5247678 | sort > obtenus-cpp.txt
	@./WordCount $(WORDS_5247678) 4 8 $(OUTPUT_FILE_RESULT) 5247678 | sort > obtenus-cpp.txt
	@java $(AVEC_SANS_JIT) -cp . WordCount $(WORDS_5247678) 4 $(OUTPUT_FILE_RESULT) 5247678 | sort > obtenus-java.txt



default: run

compile: $(EXE)
	javac WordCount.java

run: run_cpp 	run_java

run_cpp: compile
	@echo "*** Execution C++ *** "
	@./WordCount $(TEST_FILE) $(NB_TIMES) | sort > obtenus-cpp.txt
	@diff obtenus-cpp.txt $(ATTENDUS)

run_java: compile
	@echo "*** Execution Java ***"
	@java -cp . WordCount $(TEST_FILE) $(NB_TIMES) | sort > obtenus-java.txt
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


