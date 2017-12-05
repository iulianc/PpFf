//
//#include <iostream>
//#include <vector>
//#include "../../src/pp/Pipe.hpp"
//#include <sstream>
//#include <functional>
//#include <regex>
//#include <chrono>
//#include <fstream>
//
//template <typename T>
//std::string ConvertNumberToString ( T Number )
//{
// std::ostringstream ss;
// ss << Number;
// return ss.str();
//}
//
//
//
//int main(int argc, char *argv[]) {
//
//	std::string inputFile;
//	std::string outputFile;
//	std::fstream fs;
//	uint32_t iterations = 5;
//
//	//TODO if not valid path -> infinite loop
//	if(argc > 2){
//		iterations = atoi(argv[1]);
//		inputFile = argv[2];
//		std::cout << inputFile << std::endl;
//
//		if(argc > 3)
//			outputFile = argv[3];
//		std::cout << outputFile << std::endl;
//
//	}
//	else{
//		inputFile = "/home/iuly/RepositoryFastFlow/PpFf_Catch/tests/WordCount/testdata/lorem.txt";
//	}
//
//
//	std::map< std::string, int > currentResult;
//	typedef std::vector< std::string > vec_type;
//	typedef std::chrono::milliseconds ms;
//
//	auto begin = std::chrono::high_resolution_clock::now();
//
//
//
//	for(uint32_t i = 0; i < iterations; ++i)
//	{
//		pp::Pipe pipe;
//		currentResult = pipe.sourceFromFile< std::vector >(inputFile, " ")
//							.flatMap< vec_type, std::string >()
//							.map< std::string, std::string >(([](std::string data)->std::string
//									{
//										std::string result;
//										std::regex re("[^a-zA-Z]");
//										result = regex_replace(data, re , "");
//										return result;
//									}))
//							.find< std::string >(([](std::string *data)->bool {if(data->length() > 0) return true; return false;}))
//							.map< std::string, std::string >(([](std::string data)->std::string
//									{
//										transform(data.begin(), data.end(), data.begin(), [](unsigned char c){ return std::tolower(c); });
//										return data;
//									}))
//							.groupByKey< std::string, std::string, int >(0, ([](int count, std::string data)->int {return count+1;}));
//	}
//
//	auto end = std::chrono::high_resolution_clock::now();
//	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
//	std::cout << "Duration : " << duration / iterations << "ns." << std::endl;
//
//	ms duration_ms = std::chrono::duration_cast<ms>(std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin));
//	std::cout << duration_ms.count() << "ms total" << std::endl;
//	std::cout << std::endl;
//
//
//	if(!outputFile.empty()){
//		fs.open (outputFile, std::fstream::out);
//	}
//
//	std::map< std::string, int >::iterator it = currentResult.begin();
//	while(it != currentResult.end())
//	{
//		std::string currentResultKey = it->first;
//
//		int currentResultValue = it->second;
//
//		std::cout << "<" << currentResultKey << " " << ConvertNumberToString(currentResultValue) << ">" << std::endl;
//
//		if(!outputFile.empty()){
//			fs << "<" << currentResultKey << " " << ConvertNumberToString(currentResultValue) << ">" << "\n";
//		}
//
//		it++;
//	}
//
//	if(!outputFile.empty()){
//		fs.close();
//	}
//
//
//	return 0;
//}
