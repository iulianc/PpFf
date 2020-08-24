#ifndef COLLECTOR_HPP
#define COLLECTOR_HPP

namespace PpFf {

    template< typename TOperator >
    class Collector {
    public:
        typedef typename TOperator::Value Value;

        Collector()
        {}

        void addNodes(std::vector<Node*> nodes) {
            myNodes.insert(myNodes.end(), nodes.begin(), nodes.end() );
        }
        
        Value value() {
            for (unsigned int i = 1; i < this->myNodes.size(); i++) {
                *((TOperator*) myNodes[0]->op())
                    += *((TOperator*) myNodes[i]->op());
            }

            return ((TOperator*) myNodes[0]->op())->value();
        }

    private:
        std::vector<Node*> myNodes;
    };

}

#endif
