
#include <ff/node.hpp>
#include <string>
#include <sstream>
#include <iostream>

using namespace ff;

template < template < typename ELEM,
                    class ALLOC = std::allocator< ELEM > >
                    class TContainer >
class LinesFromFile: public ff_node{
public:
	LinesFromFile(const std::string& path, const std::string& delimiter): path(path), delimiter(delimiter){
		container = new TContainer< std::string >();
	}
	~LinesFromFile(){
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
