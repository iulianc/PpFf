#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <ff/ff.hpp>
#include <pipeline/Node.hpp>
#include <pipeline/Farm.hpp>
#include <operators/Empty.hpp>
#include <pipeline/Worker.hpp>
#include <stdexcept>
#include <pipeline/Collector.hpp>


namespace PpFf {

    class Pipeline {
    public:
        Pipeline() : no_workers(1)
        {}

        ~Pipeline() {
            for (unsigned int i = 0; i < myNodes.size(); i++) {
                delete myNodes[i];
            }
            
            myNodes.clear();
        }

        //////////////////////////////////////////////////////////////////
        // Diverses methodes pour creer des nodes et les ajouter au pipeline.
        //////////////////////////////////////////////////////////////////

        // count; collect; sum; flat; sort
        template< typename TOperator, typename Param1 = NULL_TYPE, typename Param2 = NULL_TYPE, typename Param3 = NULL_TYPE >
        void addNodes(int const& nb_instances) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [] { return new Node(new TOperator()); } );

            addNodes_(nodes);
        }

        // map; groupByKey
        template< typename TOperator, typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        void addNodes(int const& nb_instances, std::function<Param2*(Param1*)> const& taskFunc) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [&taskFunc] { return new Node(new TOperator(taskFunc)); } );
            addNodes_(nodes);
        }

        // groupByKey
        template< typename TOperator, typename Param1, typename Param2, typename Param3 >
        void addNodes(int const& nb_instances, std::function<Param2*(Param1*)> const& taskFunc1, std::function<Param3*(Param1*)> const& taskFunc2) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [&taskFunc1, &taskFunc2] { return new Node(new TOperator(taskFunc1, taskFunc2)); } );

            addNodes_(nodes);
        }

        // find; anyMatch; noneMatch
        template< typename TOperator, typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        void addNodes(int const& nb_instances, std::function<Param2(Param1*)> const& taskFunc) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [&taskFunc] { return new Node(new TOperator(taskFunc)); } );

            addNodes_(nodes);
        }

        // peek
        template< typename TOperator, typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        void addNodes(int const& nb_instances, std::function<void(Param1*)> const& taskFunc) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [&taskFunc] { return new Node(new TOperator(taskFunc)); } );

            addNodes_(nodes);
        }
        
        // source
        template< typename TOperator, typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        void addNodes(int const& nb_instances, Param1 &param1, Param2 &param2) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [&param1, &param2] { return new Node(new TOperator(param1, param2)); } );
            
            addNodes_(nodes);
        }

        // reduce; linesFromFile; reduceByKey
        template< typename TOperator, typename Param1, typename Param2 = NULL_TYPE, typename Param3 = NULL_TYPE >
        void addNodes(int const& nb_instances, Param1 const& param1) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [&param1] { return new Node(new TOperator(param1)); } );
            
            addNodes_(nodes);
        }

        // reduceByKey2
        template< typename TOperator, typename Param1, typename Param2, typename Param3 = NULL_TYPE >
        void addNodes(int const& nb_instances, std::function<void(Param2*, Param1*)> const& taskFunc1, std::function<void(Param2*, Param2*)> const& taskFunc2) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [&taskFunc1, &taskFunc2] { return new Node(new TOperator(taskFunc1, taskFunc2)); } );
            
            addNodes_(nodes);
        }

        // min; max;
        template< typename TOperator, typename Param1, typename Param2 = NULL_TYPE, typename Param3 = NULL_TYPE >
        void addNodes(int const& nb_instances, std::function<void(Param1*, Param1*)> taskFunc) {
            std::vector<Node*> nodes(nb_instances);
            std::generate( nodes.begin(), nodes.end(),
                           [&taskFunc] { return new Node(new TOperator(taskFunc)); } );

            addNodes_(nodes);
        }


        //////////////////////////////////////////////////////////////////
        // Autres methodes d'instance.
        //////////////////////////////////////////////////////////////////

        int nbWorkers() {
            return no_workers;
        }

        template< typename TOperator, typename T >
        T value() {
            Collector<TOperator> collector(lastNodes);
            return collector.value();
        }        

        ff_node* build_ff_node() {
            ff::ff_pipeline *pipe = new ff::ff_pipeline();
            for (Node *node : myNodes) {
                pipe->add_stage(node->build_ff_node());
            }

            return pipe;
        }

        void setNbWorkers(int no_workers) {
            if (this->no_workers < 1) {
                std::cerr << "*** Avertissement le nombre de workers ne peut pas etre nul ou negatif ***\n";
                return;
            }

            this->no_workers = no_workers;
        }

        bool isParallel() {
            return no_workers > 1;
        }        

        int run(bool = false) {
            ff::ff_pipeline *pipe = (ff::ff_pipeline*) build_ff_node();
            if (pipe->run_and_wait_end() < 0) error("running pipeline");

            return 1;
        }

    private:
        unsigned int no_workers;
        std::vector<Node*> myNodes;
        std::vector<Node*> lastNodes;
        bool sourceExists = false;

        void addNodes_(std::vector<Node*> nodes) {
            assert(nodes.size() == no_workers);

            sourceExists |= nodes[0]->isSource();

            if (!sourceExists) {
                throw std::invalid_argument( "The pipeline must have a source" );
                return;
            }

            if (nodes[0]->isCollector()) {
                lastNodes = nodes;
            }
            
            // On l'ajoute aussi dans le pipeline ou la farm fast_flow.
            Node *lastNode = myNodes.size() > 0 ? myNodes.back() : NULL;
            Farm* farm = dynamic_cast<Farm*>(lastNode);

            if ( farm == NULL ) {
                // Le dernier noeud n'est pas un Farm.
                if (isParallel()) {
                    // Execution parallele => on cree un farm.
                    farm = new Farm(no_workers);
                    myNodes.push_back(farm);
                    farm->addNodes(nodes);
                } else {
                    // Execution non parallele => pas besoin de farm.
                    myNodes.push_back(nodes[0]);
                }
            } else {
                // Le dernier noeud est deja un Farm
                if (isParallel()) {
                    if (farm->nbWorkers() != no_workers) {
                        // Le nombre de workers a change => nouveau farm.
                        farm->addCollector(new Empty());
                        farm = new Farm(no_workers);
                        myNodes.push_back(farm);
                    }
                    farm->addNodes(nodes);
                } else {
                    // Redevenu sequentiel.
                    farm->addCollector(new Empty());                    
                    myNodes.push_back(nodes[0]);
                }
            }
        }

    };

}

#endif
