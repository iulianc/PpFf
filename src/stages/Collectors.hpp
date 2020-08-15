#ifndef COLLECTORS_HPP
#define COLLECTORS_HPP

namespace PpFf {

    template< typename TOperator >
    class Collectors : public BaseStage<TOperator> {
    public:
        typedef typename TOperator::Value Value;

    	Collectors(){
            this->isFinalStage = true;
    	}

        Value value() {
            combineWorkersResults();
            
            return this->workers[0]->value();
        }

    private:
        void combineWorkersResults() {
            if (this->workers.size() > 0) {
                for (unsigned int i = 1; i < this->workers.size(); i++) {
                    *(this->workers[0]) += *(this->workers[i]);
                }
            }
        }
        
    };

}

#endif
