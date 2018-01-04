#include <ff/node.hpp>

using namespace ff;

class Close: public ff::ff_node {
public:

	void* svc(void* task) {
		return GO_ON;
	}
};
