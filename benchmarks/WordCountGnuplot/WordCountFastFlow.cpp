//#include <iostream>
//#include <ff/pipeline.hpp>
//#include <ff/farm.hpp>
//#include <functional>
//#include <regex>
//#include <chrono>
//#include <ctime>
//#include <ratio>
//#include <vector>
//#include <map>
//#include <string>
//#include <sstream>
//#include<fstream>
//#include <locale>
//#include <unordered_map>
//
//using namespace ff;
//
//#define DEFAULT_NB_ITERATIONS 10
//#define DEFAULT_INPUT_FILE "/home/iuly/WorkplaceEclipse/PpFf/tests/WordCount/testdata/78792Words.txt"
////#define DEFAULT_INPUT_FILE "testdata/78792Words.txt"
//
//
//template <typename T>
//std::string numberToString (T number) {
//    std::ostringstream ss;
//    ss << number;
//    return ss.str();
//}
//
//typedef std::vector<std::string> Words;
//
//
//struct linesFromFileStage: ff_node {
//	std::string const &path;
//
//	linesFromFileStage(std::string const &path):path(path){}
//
//    void* svc(void* task) {
//        std::ifstream file(path);
//
//        std::string* line = new std::string;
//        while (std::getline(file, *line)) {
//            ff_send_out(line);
//            line = new std::string;
//        }
//        return EOS;
//    }
//};
//
//
//struct splitInWordsStage: ff_node {
//    std::string delimiter = " ";
//    struct toLowercaseLettersStage: ff_node_t<std::string> {
//
//        std::string* svc(std::string* task) {
//        	std::string* result = new std::string;
//
//            for (auto& c: *task) {
//                int ci = (int) c;
//                if (('A' <= ci && ci <= 'Z') || ('a' <= ci && ci <= 'z'))
//                    result->push_back(c);
//            }
//            transform(result->begin(), result->end(), result->begin(),
//                      [](char c) { return ('A' <= c && c <= 'Z') ? c-('Z'-'z') : c; });
//
//            return result;
//    	}
//    };
//    void* svc(void* task) {
//    	std::string line = *((std::string*)task);
//        Words* words = new Words();
//        size_t start = 0, end = 0;
//        do {
//            end = line.find(delimiter, start);
//            size_t len = end - start;
//            words->push_back( line.substr(start, len) );
//            start += len + delimiter.length();
//        } while (end != std::string::npos);
//
//        return words;
//    }
//};
//
//
//struct flatStage: ff_node {
//    std::string delimiter = " ";
//
//	void* svc(void* task) {
//	    for(auto &elem : *(Words*)task){
//	    	ff_send_out(&elem);
//	    }
//
//		return GO_ON;
//	}
//};
//
//struct toLowercaseLettersStage: ff_node_t<std::string> {
//
//    std::string* svc(std::string* task) {
//    	std::string* result = new std::string;
//
//        for (auto& c: *task) {
//            int ci = (int) c;
//            if (('A' <= ci && ci <= 'Z') || ('a' <= ci && ci <= 'z'))
//                result->push_back(c);
//        }
//        transform(result->begin(), result->end(), result->begin(),
//                  [](char c) { return ('A' <= c && c <= 'Z') ? c-('Z'-'z') : c; });
//
//        return result;
//	}
//};
//
//struct filterEmptyWordsStage:ff_node_t<std::string> {
//
//	std::string* svc(std::string* task) {
//		if(task->length() > 0){
//			return task;
//		}
//
//        return GO_ON;
//    }
//};
//
//
//struct groupByKeyStage:ff_node {
//	typedef std::unordered_map< std::string, int > CONTAINER;
//	CONTAINER &container;
//
//	groupByKeyStage(CONTAINER &container): container(container){}
//
//	void* svc(void* task) {
//		container[*((std::string*)task)] += 1;
//		return GO_ON;
//    }
//};
//
//
//struct collectorStage:ff_node_t<std::string> {
//
//	std::string* svc(std::string* task) {
//        std::cout << *task << std::endl;
//        return GO_ON;
//    }
//};
//
//
//
//int main(int argc, char *argv[]) {
//	uint32_t nbIterations = DEFAULT_NB_ITERATIONS;
//	std::unordered_map< std::string, int > result;
//	std::string inputFile = DEFAULT_INPUT_FILE;
//
//    if (argc >= 2) {
//        inputFile = argv[1];
//    }
//
//    if (argc >= 3) {
//        nbIterations = atoi(argv[2]);
//    }
//
//	auto begin = std::chrono::high_resolution_clock::now();
//
//	for (uint32_t i = 0; i < nbIterations; ++i) {
//
//		linesFromFileStage linesFromFile(inputFile);
//		collectorStage collector;
//		splitInWordsStage splitInWords;
//		flatStage flat;
//		toLowercaseLettersStage toLowercaseLetters;
//		filterEmptyWordsStage filterEmptyWords;
//		groupByKeyStage groupByKey(result);
//
//		ff_pipeline ffp;
//		ffp.add_stage(&linesFromFile);
//		ffp.add_stage(&splitInWords);
//		ffp.add_stage(&flat);
//		ffp.add_stage(&toLowercaseLetters);
//		ffp.add_stage(&filterEmptyWords);
//		ffp.add_stage(&groupByKey);
//
//
//		if (ffp.run_and_wait_end()<0) error("running pipe");
//	}
//
//    auto end = std::chrono::high_resolution_clock::now();
//
//    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
//
//    std::cerr << "Temps C++:  " << duration_ms / nbIterations << " ms" << std::endl;
//
//
//    for (auto it = result.begin(); it != result.end(); it++) {
//        std::string currentResultKey = it->first;
//        int currentResultValue = it->second;
//        std::cout << currentResultKey << " => " << numberToString(currentResultValue) << std::endl;
//    }
//
//
//	return 0;
//}
//
//
//
//
//
//
