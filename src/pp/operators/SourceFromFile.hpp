
#include <ff/node.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace ff;

template < template < typename ELEM,
                    class ALLOC = std::allocator< ELEM > >
                    class TContainer >
class SourceFromFile: public ff_node{
public:
	SourceFromFile(const std::string& path, const std::string& delimiter): path(path), delimiter(delimiter){
		container = new TContainer< std::string >();
	}
	~SourceFromFile(){
		delete container;
		container = NULL;
	}

	void* svc(void* task) {
		std::ifstream file(path);
		std::string line;

		while (std::getline(file, line)){
		    size_t start = 0;
		    size_t end = 0;
		    size_t len = 0;

		    do{ end = line.find(delimiter,start);
		        len = end - start;
		        container->emplace_back( line.substr(start, len) );
		        start += len + delimiter.length();
		    }while ( end != std::string::npos );
		}

		this->ff_send_out(container);

		return EOS;
	}

private:
	const std::string& path;
	const std::string& delimiter;
	TContainer< std::string > *container;
};




//
//template < template < typename ELEM,
//                    class ALLOC = std::allocator< ELEM > >
//                    class TContainer >
//class SourceFromFile: public ff_node{
//public:
//	SourceFromFile(const std::string& path, const std::string& delimiter): path(path), delimiter(delimiter){}
//	~SourceFromFile(){
//		delete container;
//		container = NULL;
//	}
//
//	void* svc(void* task) {
//		std::ifstream file(path);
//		std::string line;
//
//		while (std::getline(file, line)){
//		    size_t start = 0;
//		    size_t end = 0;
//		    size_t len = 0;
//
//		    container = new TContainer< std::string >();
//
//
//		    do{ end = line.find(delimiter,start);
//		        len = end - start;
//		        container->emplace_back( line.substr(start, len) );
//		        start += len + delimiter.length();
//		    }while ( end != std::string::npos );
//
////		    while (end <= std::string::npos){
////		    	container->emplace_back(line.substr(start, end-start));
////
////		    	if (end == std::string::npos)
////		    		break;
////
////		    	start = end + 1;
////		    	end = line.find_first_of(delimiter, start);
////		    }
//
//			this->ff_send_out(container);
//		}
//
//		return EOS;
//	}
//
//private:
//	const std::string& path;
//	const std::string& delimiter;
//	TContainer< std::string > *container;
//};
