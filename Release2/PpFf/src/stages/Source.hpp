#ifndef SOURCE_HPP
#define SOURCE_HPP

#include <stages/Stage.hpp>

namespace PpFf {

    template< typename TOperator >
    class Source: public Stage<TOperator> {
    public:
    	Source(){
    		this->isSourceStage = true;
    	}
    };
}

#endif
