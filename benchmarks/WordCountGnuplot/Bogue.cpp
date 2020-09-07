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

std::string* id( std::string* s ) {
    return s;
}

int main(int argc, char* argv[]) {
    std::vector<std::string> elems =
        {
         "abc",
         "def"
        };
    
    Reducer<std::string, int> reducer(0, 
                                      [](int count, std::string s) {
                                          std::cout << "Dans accumulator: count = "
                                                    << count << "; s = '"
                                                    << s << "'"
                                                    << std::endl;
                                          return count + 1;
                                      },
                                      []( int x, int y ) {
                                          std::cout << x << " " << y << std::endl;
                                          return x + y;
                                      }
                                      );

    std::unordered_map<std::string, int> currentResult = 
        Flow
        ::source<std::string>(elems.begin(), elems.end())
        // Avec id: ok
        //.map<std::string, std::string>( id )
        // Avec toLowercaseLetters: Segmentation fault!
        .map<std::string, std::string>( toLowercaseLetters )
        .reduceByKey<std::string, std::string, int>(reducer);  
    return 0;
}
