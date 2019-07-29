.IGNORE:

N=10
N=100
#N=1000
#N=2000
#N=5000
#N=1000000
#N=10000000

#default:  run_bm_mfm
#default:  run_bm_rbk
#default: graphes_mfm
#default: graphes_rbk
default: run_maps

########################################################

# Specify the final target name
EXE 		:= FastFlowMaps Maps

# Specify the tests path
ROOT_TESTS 	:= $(wildcard *.cpp)
# Specify the pipe path
ROOT_PIPE      := $(HOME)/ProjectMemoir/Release2/PpFf/src
#ROOT_PIPE 	:= $(HOME)/PpFf/Release2/PpFf/src

# Get the corresponding object file list
OBJ 		:= $(ROOT_TESTS:.cpp=.o)
# From the object file list, get the dependency file list to handle automatic
DEP 		:= $(OBJ:.o=.d)

# Compiler
CXX		= g++ -std=c++17
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


NB_MAPS=1
N=1000
GRANULARITE=3
NB_THREADS=2

compile: $(EXE)
		javac JavaMaps.java

run_maps: compile
	#./run_maps.sh
	#@MALLOC_CHECK_=0 ./Maps 1000 1 1 1
	#@MALLOC_CHECK_=0 ./Maps 1000 1 1 2
	#@MALLOC_CHECK_=0 ./Maps 1000 1 1 4
	# Avec 1 maps et granularite = 4
	@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 4 1
	@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 4 2
	@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 4 4
	@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 4 8
	@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 4 16


run_fastflow_maps: compile
	#@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 1 1
	#@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 1 2
	#@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 1 4
	#@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 1 8
	#@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 1 16
	# Avec 1 maps et granularite = 2
	#@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 2 1
	#@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 2 2
	#@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 2 4
	#@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 2 8
	#@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 2 16
	# Avec 1 maps et granularite = 4
	@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 4 1
	@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 4 2
	@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 4 4
	@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 4 8
	@MALLOC_CHECK_=0 ./FastFlowMaps 100000 1 4 16


run_java_maps: compile
	@java -cp . JavaMaps 100000 1 4


clean: 
	\rm -f $(EXE) $(OBJ) $(DEP)
	\rm -f *.class
	\rm -f *.o *~

cleanall cleanxtra: clean
	\rm -f obtenus*txt temps-sh.txt res.txt
	\rm -f fichier_temps*txt

# Include the dependency
#-include $(DEP)

