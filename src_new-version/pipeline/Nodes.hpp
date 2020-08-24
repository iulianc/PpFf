#ifndef NODES_HPP
#define NODES_HPP

#include <functional>
#include <utilities/NullType.hpp>
#include <pipeline/Node.hpp>

namespace PpFf {

    template< typename TOperator >
    class Nodes {
    public:
        // count; collect; sum; flat; sort
        template< typename Param1 = NULL_TYPE, typename Param2 = NULL_TYPE, typename Param3 = NULL_TYPE >
        static std::vector<Node*> create(int const& nb_instances) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [] { return new Node(new TOperator()); } );

            return nodes;
        }

        // map; groupByKey
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        static std::vector<Node*> create(int const& nb_instances, std::function<Param2*(Param1*)> const& taskFunc) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [&taskFunc] { return new Node(new TOperator(taskFunc)); } );
            return nodes;
        }

        // groupByKey
        template< typename Param1, typename Param2, typename Param3 >
        static std::vector<Node*> create(int const& nb_instances, std::function<Param2*(Param1*)> const& taskFunc1, std::function<Param3*(Param1*)> const& taskFunc2) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [&taskFunc1, &taskFunc2] { return new Node(new TOperator(taskFunc1, taskFunc2)); } );

            return nodes;
        }

        // find; anyMatch; noneMatch
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        static std::vector<Node*> create(int const& nb_instances, std::function<Param2(Param1*)> const& taskFunc) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [&taskFunc] { return new Node(new TOperator(taskFunc)); } );

            return nodes;
        }

        // peek
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        static std::vector<Node*> create(int const& nb_instances, std::function<void(Param1*)> const& taskFunc) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [&taskFunc] { return new Node(new TOperator(taskFunc)); } );

            return nodes;
        }
        
        // source
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        static std::vector<Node*> create(int const& nb_instances, Param1 &param1, Param2 &param2) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [&param1, &param2] { return new Node(new TOperator(param1, param2)); } );
            
            return nodes;
        }

        // reduce; linesFromFile; reduceByKey
        template< typename Param1, typename Param2 = NULL_TYPE, typename Param3 = NULL_TYPE >
        static std::vector<Node*> create(int const& nb_instances, Param1 const& param1) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [&param1] { return new Node(new TOperator(param1)); } );
            
            return nodes;
        }

        // reduceByKey2
        template< typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        static std::vector<Node*> create(int const& nb_instances, std::function<void(Param2*, Param1*)> const& taskFunc1, std::function<void(Param2*, Param2*)> const& taskFunc2) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [&taskFunc1, &taskFunc2] { return new Node(new TOperator(taskFunc1, taskFunc2)); } );
            
            return nodes;
        }

        // min; max;
        template< typename Param1, typename Param2 = NULL_TYPE, typename Param3 = NULL_TYPE >
        static std::vector<Node*> create(int const& nb_instances, std::function<void(Param1*, Param1*)> taskFunc) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [&taskFunc] { return new Node(new TOperator(taskFunc)); } );

            return nodes;
        }
    };
}

#endif
