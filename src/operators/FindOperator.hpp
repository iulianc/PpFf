#ifndef FINDOPERATOR_HPP
#define FINDOPERATOR_HPP

#include <operators/BaseOperator.hpp>
#include <functional>


namespace PpFf {

    template < typename T >
    class FindOperator: public BaseOperator {
    public:
        FindOperator(std::function< bool(T*) > const& taskFunc): taskFunc(taskFunc) { };

        FindOperator(const FindOperator& other) : taskFunc(other.taskFunc) { }

        FindOperator(FindOperator&& other) noexcept : taskFunc(std::move(other.taskFunc)) { }

        ~FindOperator() { }

        void* svc(void* task) {
            if (!taskFunc((T*)task)) {
                //delete((T*)task);
                return (T*)GO_ON;
            } else {
                return task;
            }
        }

        
    private:
        std::function< bool(T*) > const& taskFunc;
    };

}

#endif
