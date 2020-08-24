#ifndef INTERMEDIATEOP_HPP
#define INTERMEDIATEOP_HPP

#include <ff/ff.hpp>
#include <operators/Operator.hpp>

namespace PpFf {

    class IntermediateOp : public Operator {
    public:
		IntermediateOp() { 
            operatorType = INTERMEDIATE_OP;
        }
    };

}

#endif
