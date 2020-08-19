#ifndef BASEOPERATOR_HPP
#define BASEOPERATOR_HPP

#include <ff/ff.hpp>
#include <pipeline/Node.hpp>

using namespace ff;

namespace PpFf {

    class BaseOperator : public Node {
    public:
        virtual void* svc(void * task) = 0;

        BaseOperator& operator+=( const BaseOperator& other ) {
            return *this ;
        }
    };

}

#endif
