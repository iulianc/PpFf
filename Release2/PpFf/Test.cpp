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
#include <string>
#include <sstream>
#include<fstream>
#include <locale>
#include <Pipe.hpp>
#include <unordered_map>


using namespace PpFf;



int main(int argc, char *argv[]) {

    int n = 100;
    std::vector<int> elems(n);

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    Pipe pipe;
    unsigned int currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .count();

	std::cout << "Count: " << currentResult << std::endl;


    std::vector<int> elems1(n);

    for (unsigned int i = 0; i < elems1.size(); i++) {
        elems1[i] = i;
    };

    Pipe pipe1;
    int currentResult1 = pipe1
        .source<int>(elems1.begin(), elems1.end())
        .sum<int>();

    std::cout << "Sum: " << currentResult1 << std::endl;

	return 0;
}






