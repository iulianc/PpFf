
#include <ff/node.hpp>

using namespace ff;

template < typename T, typename C >
class Collector: public ff::ff_node_t< T > {
public:
	Collector(C &container): container(container){}

	T* svc(T* task) {
		container.push(*task);

//		std::cout<<"Container Stage - Container items: ";
//		container.print();
		//delete(task);
		return (T*)GO_ON;
	}

private:
	C &container;
};
