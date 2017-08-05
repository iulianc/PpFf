
#include <ff/node.hpp>

using namespace ff;

template < typename T, typename TaskFunc >
class Peek: public ff_node_t<T> {
public:
	Peek(TaskFunc const& taskf): taskf(taskf){};
	~Peek(){}

	T* svc(T* task) {
		taskf(*task);
		return task;
	}

private:
	TaskFunc const taskf;
};
