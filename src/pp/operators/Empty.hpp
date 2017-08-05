#include <ff/node.hpp>

using namespace ff;

class Empty: public ff::ff_node {
public:

	void* svc(void* task) {
		//std::cout << "Empty Stage - Item: " << *task << "\n";

		return task;
	}
};
