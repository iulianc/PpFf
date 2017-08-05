
#include <ff/node.hpp>

using namespace ff;

template < typename In, typename Out, typename TaskFunc >
class Map: public ff_node {
public:
	Map(TaskFunc const& taskf): taskf(taskf){};
	~Map(){};

	void* svc(void* task) {
		In* in = new In();
		*in = *(In*)task;
		return new Out(taskf(*in));
	}


private:
	TaskFunc const taskf;
};
