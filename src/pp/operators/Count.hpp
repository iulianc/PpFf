
#include <ff/node.hpp>
#include "../utilities/Accumulator.hpp"

using namespace ff;

template < class Accum = Accumulator< unsigned int > >
class Count: public ff::ff_node {
public:
	Count(Accum &accum): accum(accum){}

	void* svc(void* task) {
		accum.accumulate(1);

		return GO_ON;
	}

private:
	Accum &accum;
};
