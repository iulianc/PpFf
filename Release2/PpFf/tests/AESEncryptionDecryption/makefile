#def:
#	make -f gt-makefile



# Specify the final target name
EXE 		:= encrypt_decrypt aes256

# Specify the tests path
ROOT_TESTS 	:= $(wildcard *.cpp)
# Specify the pipe path
ROOT_PIPE      := $(HOME)/ProjectMemoir/Release2/PpFf/src

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

FILE_ENCRYPTED="testdata/text-encrypted.txt"
INPUT_FILE="testdata/text-to-encrypt.txt"
FILE_DECRYPTED="testdata/text-decrypted.txt"
ENCRYPT = "encrypt"
DECRYPT = "decrypt"
KEY="testdata/key.txt"
NO_ENC_DEC=1
NO_TIMES=1
PPFF="PpFf"
CPP_NATIVE="cpp-native"

# Tell make that these target are not real files
.PHONY: all clean cleanall
.SUFFIXES: .cpp 

default: run

run_encryption_native: encrypt_native
run_decryption_native: decrypt_native
run_encryption_ppff: encrypt_ppff
run_decryption_ppff: decrypt_ppff

encrypt_native: compile
	@echo "*** Execution C++ native - Encryption *** "
	echo "" > $(FILE_ENCRYPTED)
	@./encrypt_decrypt $(KEY)  $(INPUT_FILE) $(FILE_ENCRYPTED) $(ENCRYPT) $(NO_ENC_DEC) $(NO_TIMES) $(CPP_NATIVE)
	@echo ""

decrypt_native: compile
	@echo "*** Execution C++ native - Decryption  *** "
	echo "" > $(FILE_DECRYPTED)
	@./encrypt_decrypt $(KEY) $(FILE_ENCRYPTED) $(FILE_DECRYPTED) $(DECRYPT) $(NO_ENC_DEC) $(NO_TIMES) $(CPP_NATIVE)
	@diff $(INPUT_FILE) $(FILE_DECRYPTED)
	@echo ""


encrypt_ppff: compile
	@echo "*** Execution PpFf - Encryption *** "
	echo "" > $(FILE_ENCRYPTED)
	@./encrypt_decrypt $(KEY)  $(INPUT_FILE) $(FILE_ENCRYPTED) $(ENCRYPT) $(NO_ENC_DEC) $(NO_TIMES) $(PPFF)
	@echo ""


decrypt_ppff: compile
	@echo "*** Execution PpFf - Decryption *** "
	echo "" > $(FILE_DECRYPTED)
	@./encrypt_decrypt $(KEY) $(FILE_ENCRYPTED) $(FILE_DECRYPTED) $(DECRYPT) $(NO_ENC_DEC) $(NO_TIMES) $(PPFF)
	@diff $(INPUT_FILE) $(FILE_DECRYPTED)
	@echo ""


compile	: $(EXE)

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


