
#include <ff/node.hpp>
#include "../utilities/Accumulator.hpp"

using namespace ff;

template < typename T, class Accum = Accumulator< T > >
class Sum: public ff::ff_node_t< T > {
public:
	Sum(Accum *accum): accum(accum){}

	T* svc(T* task) {
		accum->accumulate(*task);
//		std::cout << "Sum Stage - Partial sum: " << accum->value() << "\n";
		return (T*)GO_ON;
	}

private:
	Accum *accum;
};



