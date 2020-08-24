#ifndef FLATMAPOPERATOR_HPP
#define FLATMAPOPERATOR_HPP

#include <ff/ff.hpp>
#include <functional>
#include <operators/Operator.hpp>

namespace PpFf {

    template < typename In, typename TContainer, typename Out >
    class FlatMapOperator: public Operator {

    public:
        FlatMapOperator(std::function< TContainer*(In*) > const& taskFunc) : taskFunc(taskFunc) 
        {}

        FlatMapOperator(const FlatMapOperator& other) : taskFunc(other.taskFunc) 
        {}

        FlatMapOperator(FlatMapOperator&& other) noexcept : taskFunc(std::move(other.taskFunc)) 
        {}

        FlatMapOperator() 
        {}

        ~FlatMapOperator() 
        {}
            
        void* svc(void* task) {
            for (auto &elem : (*(TContainer*) taskFunc((In*)task))) {
                this->ff_send_out(&elem);
            }
            return GO_ON;
        }

    private:
        std::function< TContainer*(In*) > const& taskFunc;
    };

}

#endif
