#ifndef COLLECTOROP_HPP
#define COLLECTOROP_HPP

#include <ff/ff.hpp>
#include <operators/Operator.hpp>

namespace PpFf {

    class CollectorOp : public Operator {
    public:
		CollectorOp() { 
            operatorType = COLLECTOR_OP;
        }
    };

}

#endif
