#ifndef BASESTAGE_HPP
#define BASESTAGE_HPP

#include <functional>
#include <utilities/NullType.hpp>
#include <stages/IStage.hpp>

namespace PpFf {

    template< typename TOperator >
    class BaseStage: public IStage {
    public:
        bool const isFinalStage = false;
        std::vector<TOperator*> workers;

        ~BaseStage() {
            for (unsigned int i = 0; i < workers.size(); i++) {
                delete (workers[i]);
            }

            workers.clear();
        }

        //count; collect; sum; flat; sort
        template< typename Param1 = NULL_TYPE, typename Param2 = NULL_TYPE, typename Param3 = NULL_TYPE >
        void createOperators(int const& no_workers) {
            for (int i = 0; i < no_workers; i++) {
                workers.push_back(new TOperator());
            }
        }

        //map; groupByKey
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        void createOperators(int const& no_workers, std::function< Param2*(Param1*) > const& taskFunc) {
            for (int i = 0; i < no_workers; i++) {
                workers.push_back(new TOperator(taskFunc));
            }
        }

        //groupByKey
        template< typename Param1, typename Param2, typename Param3 >
        void createOperators(int const& no_workers, std::function< Param2*(Param1*) > const& taskFunc1, std::function< Param3*(Param1*) > const& taskFunc2) {
            for (int i = 0; i < no_workers; i++) {
                workers.push_back(new TOperator(taskFunc1, taskFunc2));
            }
        }

        //find; anyMach; noneMach
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        void createOperators(int const& no_workers, std::function< Param2(Param1*) > const& taskFunc) {
            for (int i = 0; i < no_workers; i++) {
                workers.push_back(new TOperator(taskFunc));
            }
        }

        //peek
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        void createOperators(int const& no_workers, std::function< void(Param1*) > const& taskFunc) {
            for (int i = 0; i < no_workers; i++) {
                workers.push_back(new TOperator(taskFunc));
            }
        }

        //source
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        void createOperators(int const& no_workers, Param1 const& param1, Param2 const& param2) {
            for (int i = 0; i < no_workers; i++) {
                workers.push_back(new TOperator(param1, param2));
            }
        }

        //reduce; linesFromFile; reduceByKey
        template< typename Param1, typename Param2 = NULL_TYPE, typename Param3 = NULL_TYPE >
        void createOperators(int const& no_workers, Param1 const& param1) {
            for (int i = 0; i < no_workers; i++) {
                workers.push_back(new TOperator(param1));
            }
        }

        //reduceByKey2
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        void createOperators(int const& no_workers, std::function< void(Param2*, Param1*) > const& taskFunc1, std::function< void(Param2*, Param2*) > const& taskFunc2) {
            for (int i = 0; i < no_workers; i++) {
                workers.push_back(new TOperator(taskFunc1, taskFunc2));
            }
        }

        //min; max;
        template< typename Param1, typename Param2 = NULL_TYPE, typename Param3 = NULL_TYPE >
        void createOperators(int const& no_workers, std::function< void(Param1*, Param1*) > taskFunc) {
            for (int i = 0; i < no_workers; i++) {
                workers.push_back(new TOperator(taskFunc));
            }
        }
    };
}

#endif
