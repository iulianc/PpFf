#ifndef SOURCEOPERATOR_HPP
#define SOURCEOPERATOR_HPP

#include <ff/ff.hpp>
#include <functional>
#include <ff/ff.hpp>
#include <operators/Operator.hpp>

namespace PpFf {

    template< typename T, typename It, bool Preserve = false >
    class SourceOperator: public Operator {};

    template< typename T, typename It >
    class SourceOperator< T, It, false >: public Operator {
    public:
        SourceOperator(It &begin, It &end): begin(begin), end(end)
        { operatorType = SOURCE_OP; }
        
        SourceOperator(const SourceOperator& other): begin(other.begin), end(other.end)
        { operatorType = SOURCE_OP; }

        SourceOperator(SourceOperator&& other) noexcept: begin(std::move(other.begin)), end(std::move(other.end))
        { operatorType = SOURCE_OP; }

        ~SourceOperator()
        {}

        void* svc(void* task) {
            for (; begin != end; begin++) {
                this->ff_send_out(new T(*begin));
            }

            return EOS;
        }

    private:
        It begin;
        It end;
    };


    template< typename T, typename It >
    class SourceOperator< T*, It, false >: public Operator {
    public:
        SourceOperator(It const& begin, It const& end): begin(begin), end(end)
        { operatorType = SOURCE_OP; }

        SourceOperator(const SourceOperator& other): begin(other.begin), end(other.end)
        { operatorType = SOURCE_OP; }

        SourceOperator(SourceOperator&& other) noexcept: begin(std::move(other.begin)), end(std::move(other.end))
        { operatorType = SOURCE_OP; }

        ~SourceOperator()
        {}

        void* svc(void* task) {
            for (; begin != end; begin++) {
                this->ff_send_out(&(*begin));
            }

            return EOS;
        }

    private:
        It begin;
        It end;
    };

}

#endif
