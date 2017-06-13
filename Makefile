FF_ROOT		= $(HOME)/Repository/PpFf/pp	#directory for FastFlow package

CXX		= g++ -std=c++11
INCLUDES	= -I $(PP_ROOT) $(FF_ROOT) 
CXXFLAGS  	= -Wall #-DBLOCKING_MODE #-DNO_DEFAULT_MAPPING

LDFLAGS 	=  -pthread
OPTFLAGS	=  -O3 -finline-functions

TARGETS		= PpFf

.PHONY: all clean cleanall
.SUFFIXES: .cpp 


%: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) -o $@ $< $(LDFLAGS)

all		: $(TARGETS)
clean		: 
	rm -f $(TARGETS)
cleanall	: clean
	\rm -f *.o *~

all: $(TARGETS)
