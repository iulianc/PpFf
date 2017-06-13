
#include <ff/node.hpp>

using namespace ff;

template < typename T >
class Collector: ff_node{
public:
	Collector(T* result): result(result){};
	~Collector(){
		result = NULL;
	};

	T* svc(T* task) {
		*result = *task;

		std::cout<<"Collector Stage - Recived item: "<<*result<<"\n";

		result++;
		return GO_ON;
	}

private:
	T* result;
};
