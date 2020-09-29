
def: students_cpp


.IGNORE:

ROOT_PIPE 	:= $(HOME)/PpFf/src

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


#########
students_cpp: StudentsExample
	@echo "*** Execution C++ *** "
	@./StudentsExample

StudentsExample: StudentsExample.o
		  $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OPTFLAGS) StudentsExample.o $(LDLIBS) -o StudentsExample

students_java: StudentsExample.class
	java -cp . StudentsExample

StudentsExample.class: StudentsExample.java
	javac StudentsExample.java

clean: 
	\rm -f *.class StudentsExample
	\rm -f *.o *~

cleanx cleanall cleanxtra: clean

