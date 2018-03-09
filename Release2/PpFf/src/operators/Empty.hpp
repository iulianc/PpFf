#ifndef EMPTY_HPP
#define EMPTY_HPP

#include <ff/node.hpp>

using namespace ff;

namespace PpFf{

	class Empty: public ff::ff_node {
	public:

		void* svc(void* task) {
			return task;
		}
	};

}

#endif
