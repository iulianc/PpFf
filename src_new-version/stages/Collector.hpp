#ifndef COLLECTOR_HPP
#define COLLECTOR_HPP

namespace PpFf {

    template< typename TOperator >
    class Collector {
    public:
        typedef typename TOperator::Value Value;

        void addOperators(std::vector<Node*> nodes) {
            for(Node *node : nodes){
                operators.push_back(node);
            }
        }

        Value value() {
            for (unsigned int i = 1; i < this->operators.size(); i++) {
                *((TOperator*) (this->operators[0])->build_ff_node()) += *((TOperator*) (this->operators[i])->build_ff_node());
            }

            return ((TOperator*) (this->operators[0])->build_ff_node())->value();
        }

    private:
        std::vector<Node*> operators;
    };

}

#endif
