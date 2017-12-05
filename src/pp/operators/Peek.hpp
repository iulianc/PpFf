
#include <ff/node.hpp>
#include <functional>

using namespace ff;

template < typename T >
class Peek: public ff_node_t<T> {
public:
	Peek(std::function< void(T*) > const& taskFunc): taskFunc(taskFunc){};
	~Peek(){}

	T* svc(T* task) {
		taskFunc(task);
		return task;
	}

private:
	std::function< void(T*) > const &taskFunc;
};
