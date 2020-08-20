#ifndef COLLECTOR_STAGE_HPP
#define COLLECTOR_STAGE_HPP

#include <stages/BaseStage.hpp>

namespace PpFf {

    template< typename TOperator >
    class CollectorStage : public BaseStage<TOperator, COLLECTOR_STAGE> {
    public:
        typedef typename TOperator::Value Value;

    	CollectorStage() {
    	}

        Value value() {
            for (unsigned int i = 1; i < this->operators.size(); i++) {
                *(this->operators[0]) += *(this->operators[i]);
            }
            
            return this->operators[0]->value();
        }
        
    };

}

#endif
