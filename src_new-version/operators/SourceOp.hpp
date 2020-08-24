#ifndef SOURCEOP_HPP
#define SOURCEOP_HPP

#include <ff/ff.hpp>
#include <operators/Operator.hpp>

namespace PpFf {

    class SourceOp : public Operator {
    public:
		SourceOp() { 
            operatorType = SOURCE_OP;
        }
    };

}

#endif
