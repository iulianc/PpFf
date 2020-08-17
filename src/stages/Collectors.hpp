#ifndef COLLECTORS_HPP
#define COLLECTORS_HPP

#include <stages/BaseStage.hpp>

namespace PpFf {

    template< typename TOperator >
    class Collectors : public BaseStage<TOperator> {
    public:
        typedef typename TOperator::Value Value;

    	Collectors(){
            this->isFinalStage = true;
    	}

        Value value() {
            combineOperatorsResults();
            
            return this->operators[0]->value();
        }

    private:
        void combineOperatorsResults() {
            if (this->operators.size() > 0) {
                for (unsigned int i = 1; i < this->operators.size(); i++) {
                    *(this->operators[0]) += *(this->operators[i]);
                }
            }
        }
        
    };

}

#endif
