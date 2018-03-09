#ifndef FLATOPERATOR_HPP
#define FLATOPERATOR_HPP

#include <operators/BaseOperator.hpp>
#include <functional>

namespace PpFf{

	template < typename TContainer, typename Out >
	class FlatOperator: public BaseOperator {
	public:
		FlatOperator() { }
		~FlatOperator() { }

		void* svc(void* task) {
			for(auto &elem : *(TContainer*)task){
				this->ff_send_out(&elem);
			}

			return GO_ON;
		}
	};

}

#endif
