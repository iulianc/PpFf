
#include <ff/node.hpp>
#include <functional>

using namespace ff;

template < typename T >
class Reduce: public ff_node {
public:
	Reduce(T &identity, std::function< T(T, T) > const& biOp): identity(identity), biOp(biOp){};
	~Reduce(){};

	void* svc(void* task) {
//		T* in = new T();
//		*in = *(T*)task;
		identity = biOp(identity, *(T*)task);

		return GO_ON;
	}

private:
	T &identity;
	std::function< T(T, T) > const biOp;
};
