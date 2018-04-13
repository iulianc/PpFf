.IGNORE:

default:  run

########################################################

# Specify the final target name
EXE 		:= MapFilterMap MapFilterMapFastFlow MapFilterMapFastFlowSansMalloc

# Specify the tests path
ROOT_TESTS 	:= $(wildcard *.cpp)
# Specify the pipe path
#ROOT_PIPE      := $(HOME)/RepositoryFastFlow/PpFf_Catch/src/pp
ROOT_PIPE 	:= $(HOME)/PpFf/Release2/PpFf/src

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

compile: $(EXE)

N=10000000
N=1000000
N=20000

run: compile
	@echo "*** Execution MapFilterMap *** "
	@./MapFilterMap $(N) 1
	@./MapFilterMap $(N) 2
	@./MapFilterMap $(N) 4
	@#./MapFilterMap $(N) 8
	@echo ""
	@echo "*** Execution FastFlow *** "
	./MapFilterMapFastFlow $(N)
	@echo ""
	@echo "*** Execution MapFilterMapFastFlowSansMalloc *** "
	./MapFilterMapFastFlowSansMalloc $(N)

clean: 
	\rm -f $(EXE) $(OBJ) $(DEP)
	\rm -f *.class

cleanall cleanxtra: clean
	\rm -f *.o *~
	\rm -f obtenus*txt temps-sh.txt res.txt

# Include the dependency
#-include $(DEP)

