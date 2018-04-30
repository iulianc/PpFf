#include <stages/BaseStage.hpp>

namespace PpFf {

    template< typename TOperator >
    class BaseCollectors: public BaseStage<TOperator> {
    public:
    	BaseCollectors(){
    		this->isFinalStage = true;
    	}
    };

}
