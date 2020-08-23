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
                *((TOperator*) (this->operators[0])->build_ff_node()) += *((TOperator*) (this->operators[i])->build_ff_node());
            }

            return ((TOperator*) (this->operators[0])->build_ff_node())->value();
        }
        
    };

}

#endif
