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
EXE 		:= Maps MapFilterMap MapFilterMapFastFlow MapFilterMapFastFlowSansMalloc ReduceByKey

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
	@javac MapFilterMapJava.java
	@javac ReduceByKeyJava.java

run_java_mfm: compile
	@echo "*** Execution Java *** "
	java MapFilterMapJava $(N)
	@echo ""

run_bm_mfm: compile
	@./run_bm_mfm.rb $(N) | tee fichier_temps_$(N).txt


graphes_mfm: run_bm_mfm
	./gnuplot-temps-mfm.sh $(N)
	./gnuplot-acc-mfm.sh $(N) 10

run_mfm: compile
	@echo "*** Execution MapFilterMap *** "
	@./MapFilterMap $(N) 0
	@./MapFilterMap $(N) 1
	@./MapFilterMap $(N) 2
	@./MapFilterMap $(N) 4
	@./MapFilterMap $(N) 8
	@./MapFilterMap $(N) 16
	@echo ""
	@echo "*** Execution FastFlow *** "
	./MapFilterMapFastFlow $(N)
	@echo ""
	@echo "*** Execution MapFilterMapFastFlowSansMalloc *** "
	./MapFilterMapFastFlowSansMalloc $(N)
	@echo ""
	@echo "*** Execution Java *** "
	java MapFilterMapJava $(N)
	@echo ""

NB_MAPS=1
N=1000
GRANULARITE=3
NB_THREADS=2

run_maps: compile
	@echo "*** Execution Maps *** "
	# Avec 1 map et granularite = 1
	@MALLOC_CHECK_=0 ./Maps 1000 1 1 0
	@MALLOC_CHECK_=0 ./Maps 1000 1 1 1
	@MALLOC_CHECK_=0 ./Maps 1000 1 1 2
	@MALLOC_CHECK_=0 ./Maps 1000 1 1 4
	# Avec 2 maps et granularite = 2
	@MALLOC_CHECK_=0 ./Maps 1000 2 2 0
	@MALLOC_CHECK_=0 ./Maps 1000 2 2 1
	@MALLOC_CHECK_=0 ./Maps 1000 2 2 2
	@MALLOC_CHECK_=0 ./Maps 1000 2 2 4
	# Avec 3 maps et granularite = 3
	@MALLOC_CHECK_=0 ./Maps 1000 3 3 0
	@MALLOC_CHECK_=0 ./Maps 1000 3 3 1
	@MALLOC_CHECK_=0 ./Maps 1000 3 3 2
	@MALLOC_CHECK_=0 ./Maps 1000 3 3 4
	# Avec 5 maps et granularite = 2
	@MALLOC_CHECK_=0 ./Maps 1000 5 2 0
	@MALLOC_CHECK_=0 ./Maps 1000 5 2 1
	@MALLOC_CHECK_=0 ./Maps 1000 5 2 2
	@MALLOC_CHECK_=0 ./Maps 1000 5 2 4
	# Avec 5 maps et granularite = 6
	@MALLOC_CHECK_=0 ./Maps 1000 5 6 0
	@MALLOC_CHECK_=0 ./Maps 1000 5 6 1
	@MALLOC_CHECK_=0 ./Maps 1000 5 6 2
	@MALLOC_CHECK_=0 ./Maps 1000 5 6 4
	@echo ""
	@#@echo "*** Execution FastFlow *** "
	@#./MapsFastFlow $(N)
	@#@echo ""
	@#@echo "*** Execution Java *** "
	@#java MapsJava $(N)
	@#@echo ""

run_rbk: compile
	@echo "*** Execution ReduceByKey *** "
	@./ReduceByKey $(N) 0
	@./ReduceByKey $(N) 1
	@./ReduceByKey $(N) 2
	@#./ReduceByKey $(N) 4
	@#./ReduceByKey $(N) 8
	@#./ReduceByKey $(N) 16
	@#echo ""
	@echo "*** Execution Java *** "
	@java -enableassertions ReduceByKeyJava $(N)

run_bm_rbk: compile
	@./run_bm_rbk.rb $(N) | tee fichier_temps_$(N).txt

graphes_rbk: run_bm_rbk
	./gnuplot-temps-rbk.sh $(N)
	./gnuplot-acc-rbk.sh $(N) 4

clean: 
	\rm -f $(EXE) $(OBJ) $(DEP)
	\rm -f *.class
	\rm -f *.o *~

cleanall cleanxtra: clean
	\rm -f obtenus*txt temps-sh.txt res.txt
	\rm -f fichier_temps*txt

# Include the dependency
#-include $(DEP)

