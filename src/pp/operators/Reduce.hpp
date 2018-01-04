
#include <ff/node.hpp>
#include <functional>

using namespace ff;

template < typename T >
class Reduce: public ff_node_t<T> {
public:
	Reduce(T &identity, std::function< void(T*, T*) > const& biOp): identity(&identity), biOp(biOp){};
	~Reduce(){};

	T* svc(T* task) {
		biOp(identity, task);

		delete task;
		return (T*)GO_ON;
	}

private:
	T *identity;
	std::function< void(T*, T*) > const &biOp;
};
