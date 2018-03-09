#ifndef FINALOPERATOR_HPP
#define FINALOPERATOR_HPP

#include <operators/BaseOperator.hpp>

namespace PpFf{

	class FinalOperator : public BaseOperator {
	public:
		bool const isFinalOperator = true;
		virtual void* svc(void *task) = 0;
	};

}

#endif
