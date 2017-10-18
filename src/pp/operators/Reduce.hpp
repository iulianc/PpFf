
#include <ff/node.hpp>

using namespace ff;

template < typename In, typename TaskFunc >
class Reduce: public ff_node {
public:
	Reduce(In &identity, TaskFunc const& taskf): identity(identity), taskf(taskf){};
	~Reduce(){};

	void* svc(void* task) {
//		In* in = new In();
//		*in = *(In*)task;
		identity = taskf(identity, *(In*)task);
		return GO_ON;
	}

private:
	In &identity;
	TaskFunc const taskf;
};
