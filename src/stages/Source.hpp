#ifndef SOURCE_HPP
#define SOURCE_HPP

#include <stages/BaseStage.hpp>

namespace PpFf {

    template< typename TOperator >
    class Source: public BaseStage<TOperator> {
    public:
    	Source(){
    		this->isSourceStage = true;
    	}
    };
}

#endif
