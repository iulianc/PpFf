#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include <ff/ff.hpp>

namespace PpFf {

    enum OperatorType { SOURCE_OP, INTERMEDIATE_OP, COLLECTOR_OP };

    class Operator : public ff::ff_node {
    public:
        bool isSource() {
            return operatorType == SOURCE_OP;
        }

        bool isCollector() {
            return operatorType == COLLECTOR_OP;
        }
		
    protected:
        OperatorType operatorType;
    };

}

#endif
