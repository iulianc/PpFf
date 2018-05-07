#ifndef BASECOLLECTORS_HPP
#define BASECOLLECTORS_HPP

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

#endif
