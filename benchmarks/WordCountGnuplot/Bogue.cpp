#include <iostream>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include <functional>
#include <regex>
#include <chrono>
#include <ctime>
#include <ratio>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include<fstream>
#include <locale>
#include <Flow.hpp>
#include <unordered_map>
#include <ctype.h>

using namespace PpFf;

#define DEFAULT_INPUT_FILE "testdata/78792Words.txt"

typedef std::vector<std::string> Words;

#include "auxiliary-functions.hpp"

int main(int argc, char* argv[]) {
    std::string inputFile = argc >= 3 ? argv[2] : DEFAULT_INPUT_FILE;
    
    Reducer<std::string, int> reducer(0, 
                                      [](int count, std::string s) {
                                          std::cout << "Dans accumulator: count = "
                                                    << count << "; s = '"
                                                    << s << "'"
                                                    << std::endl;
                                          return count + 1;
                                      },
                                      std::plus<int>() );

    std::unordered_map<std::string, int> currentResult = 
        Flow
        ::source(inputFile)
        .flatMap<std::string, Words, std::string>(splitInWords)			
        .reduceByKey<std::string, std::string, int>(reducer);  
    return 0;
}
