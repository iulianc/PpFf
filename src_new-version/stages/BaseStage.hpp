#ifndef BASESTAGE_HPP
#define BASESTAGE_HPP

#include <functional>
#include <utilities/NullType.hpp>
#include <stages/Stage.hpp>
#include <pipeline/Node.hpp>

namespace PpFf {

    template< typename TOperator, StageType stageType_ = INTERMEDIATE_STAGE >
    class BaseStage: public Stage {
    public:
        std::vector<Node*> operators;

        BaseStage() {
            stageType = stageType_;
        }

        ~BaseStage() {
            for (unsigned int i = 0; i <operators.size(); i++) {
                delete operators[i];
            }
            operators.clear();
        }

        //count; collect; sum; flat; sort
        template< typename Param1 = NULL_TYPE, typename Param2 = NULL_TYPE, typename Param3 = NULL_TYPE >
        void addOperator(int const& nb_instances) {
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator()));
            }
        }

        //map; groupByKey
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        void addOperator(int const& nb_instances, std::function<Param2*(Param1*)> const& taskFunc) {
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator(taskFunc)));
            }
        }

        //groupByKey
        template< typename Param1, typename Param2, typename Param3 >
        void addOperator(int const& nb_instances, std::function<Param2*(Param1*)> const& taskFunc1, std::function<Param3*(Param1*)> const& taskFunc2) {
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator(taskFunc1, taskFunc2)));
            }
        }

        //find; anyMach; noneMach
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        void addOperator(int const& nb_instances, std::function<Param2(Param1*)> const& taskFunc) {
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator(taskFunc)));
            }
        }

        //peek
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        void addOperator(int const& nb_instances, std::function<void(Param1*)> const& taskFunc) {
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator(taskFunc)));
            }
        }
        
        //source
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        void addOperator(int const& nb_instances, Param1 &param1, Param2 &param2) {
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator(param1, param2)));
            }
        }

        //reduce; linesFromFile; reduceByKey
        template< typename Param1, typename Param2 = NULL_TYPE, typename Param3 = NULL_TYPE >
        void addOperator(int const& nb_instances, Param1 const& param1) {
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator(param1)));
            }
        }

        //reduceByKey2
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        void addOperator(int const& nb_instances, std::function<void(Param2*, Param1*)> const& taskFunc1, std::function<void(Param2*, Param2*)> const& taskFunc2) {
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator(taskFunc1, taskFunc2)));
            }
        }

        //min; max;
        template< typename Param1, typename Param2 = NULL_TYPE, typename Param3 = NULL_TYPE >
        void addOperator(int const& nb_instances, std::function<void(Param1*, Param1*)> taskFunc) {
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator(taskFunc)));
            }
        }
    };
}

#endif
