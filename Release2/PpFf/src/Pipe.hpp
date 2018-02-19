
#include <operators/CountOperator.hpp>
#include <operators/SumOperator.hpp>
#include <operators/SourceOperator.hpp>
#include <pipeline/Pipeline.hpp>
#include <stages/Stage.hpp>
#include <stages/Collectors.hpp>

using namespace ff;

namespace PpFf{

	class Pipe{
	public:
		~Pipe(){};

		template < typename T, typename Iterator >
		Pipe& source(Iterator begin, Iterator end){
			typedef SourceOperator<T, Iterator> Source;
			typedef Stage< Source > StageCount;

			StageCount *stage = pipe.createStage< StageCount >();
			stage->createOperators(pipe.getWorkers(), begin, end);
			pipe.addStage(stage);
			return *this;
		}

		unsigned int count(){
            typedef CountOperator<int> Count;
            typedef Collectors< Count > StageCollectors;

            StageCollectors *collectors = pipe.createStage< StageCollectors >();
            collectors->createOperators(pipe.getWorkers());
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
		}

		template < typename T >
		T sum(){
            typedef SumOperator<int> Sum;
            typedef Collectors< Sum > StageCollectors;

            StageCollectors *collectors = pipe.createStage< StageCollectors >();
            collectors->createOperators(pipe.getWorkers());
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
		}

	private:
		Pipeline pipe;
	};

}
