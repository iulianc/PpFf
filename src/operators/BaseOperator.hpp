#ifndef BASEOPERATOR_HPP
#define BASEOPERATOR_HPP

//#include <ff/node.hpp>
#include <ff/ff.hpp>

using namespace ff;

namespace PpFf {

    class BaseOperator : public ff_node {
    public:
        virtual void* svc(void * task) = 0;

        BaseOperator& operator+=( const BaseOperator& other ) {
            return *this ;
        }
    };

}

#endif
