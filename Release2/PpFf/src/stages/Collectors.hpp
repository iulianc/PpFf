
#include <stages/BaseCollectors.hpp>

namespace PpFf{

	template< typename TOperator >
	class Collectors : public BaseCollectors< TOperator > {
	public:
		typedef typename TOperator::Value Value;

		void reduce(){
			if(this->workers.size() > 0){
				for(unsigned int i = 1; i < this->workers.size(); i++){
					this->workers[0] += this->workers[i];
				}
			}
		}

		Value value(){
			reduce();
			return this->workers[0].value();
		}
	};
}
