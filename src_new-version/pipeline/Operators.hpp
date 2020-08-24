#ifndef OPERATORS_HPP
#define OPERATORS_HPP

#include <functional>
#include <utilities/NullType.hpp>
#include <pipeline/Node.hpp>

namespace PpFf {

    template< typename TOperator >
    class Operators {
    public:
        // count; collect; sum; flat; sort
        template< typename Param1 = NULL_TYPE, typename Param2 = NULL_TYPE, typename Param3 = NULL_TYPE >
        static std::vector<Node*> create(int const& nb_instances) {
            std::vector<Node*> operators;
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator()));
            }

            return operators;
        }

        // map; groupByKey
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        static std::vector<Node*> create(int const& nb_instances, std::function<Param2*(Param1*)> const& taskFunc) {
            std::vector<Node*> operators;
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator(taskFunc)));
            }

            return operators;
        }

        // groupByKey
        template< typename Param1, typename Param2, typename Param3 >
        static std::vector<Node*> create(int const& nb_instances, std::function<Param2*(Param1*)> const& taskFunc1, std::function<Param3*(Param1*)> const& taskFunc2) {
            std::vector<Node*> operators;
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator(taskFunc1, taskFunc2)));
            }

            return operators;
        }

        // find; anyMatch; noneMatch
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        static std::vector<Node*> create(int const& nb_instances, std::function<Param2(Param1*)> const& taskFunc) {
            std::vector<Node*> operators;
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator(taskFunc)));
            }

            return operators;
        }

        // peek
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        static std::vector<Node*> create(int const& nb_instances, std::function<void(Param1*)> const& taskFunc) {
            std::vector<Node*> operators;
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator(taskFunc)));
            }
            
            return operators;
        }
        
        // source
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        static std::vector<Node*> create(int const& nb_instances, Param1 &param1, Param2 &param2) {
            std::vector<Node*> operators;
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator(param1, param2)));
            }
            
            return operators;
        }

        // reduce; linesFromFile; reduceByKey
        template< typename Param1, typename Param2 = NULL_TYPE, typename Param3 = NULL_TYPE >
        static std::vector<Node*> create(int const& nb_instances, Param1 const& param1) {
            std::vector<Node*> operators;
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator(param1)));
            }
            
            return operators;
        }

        // reduceByKey2
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        static std::vector<Node*> create(int const& nb_instances, std::function<void(Param2*, Param1*)> const& taskFunc1, std::function<void(Param2*, Param2*)> const& taskFunc2) {
            std::vector<Node*> operators;
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator(taskFunc1, taskFunc2)));
            }
            
            return operators;
        }

        // min; max;
        template< typename Param1, typename Param2 = NULL_TYPE, typename Param3 = NULL_TYPE >
        static std::vector<Node*> create(int const& nb_instances, std::function<void(Param1*, Param1*)> taskFunc) {
            std::vector<Node*> operators;
            for (int i = 0; i < nb_instances; i++) {
                operators.push_back(new Node(new TOperator(taskFunc)));
            }
            
            return operators;
        }
    };
}

#endif
