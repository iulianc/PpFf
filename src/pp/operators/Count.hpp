
#include <ff/node.hpp>
#include "../utilities/Accumulator.hpp"

using namespace ff;

template < class Accum = Accumulator< unsigned int > >
class Count: public ff::ff_node {
public:

	Count(Accum &accum): accum(accum){}

	void* svc(void* task) {
		accum.accumulate(1);

		//std::cout << "Count Stage - Counted items: " << accum.value() << "\n";

		//delete(task);	//TODO - Add parametre template for the element counted
		return GO_ON;
	}

private:
	Accum &accum;
};
