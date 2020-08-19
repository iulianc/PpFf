#ifndef COLLECTOR_STAGE_HPP
#define COLLECTOR_STAGE_HPP

#include <stages/BaseStage.hpp>

namespace PpFf {

    template< typename TOperator >
    class CollectorStage : public BaseStage<TOperator> {
    public:
        typedef typename TOperator::Value Value;

    	CollectorStage(){
            this->isFinalStage = true;
    	}

        Value value() {
            combineIntermediateResults();
            
            return this->operators[0]->value();
        }

    private:
        void combineIntermediateResults() {
            if (this->operators.size() > 0) {
                for (unsigned int i = 1; i < this->operators.size(); i++) {
                    *(this->operators[0]) += *(this->operators[i]);
                }
            }
        }
        
    };

}

#endif
