#ifndef BASESTAGE_HPP
#define BASESTAGE_HPP

#include <functional>
#include <utilities/NullType.hpp>
#include <stages/IStage.hpp>

namespace PpFf{

	template< typename TOperator >
	class BaseStage : public IStage {
	public:
		bool const isFinalStage = false;
		std::vector< TOperator* > workers;

		~BaseStage(){
			for (unsigned int i = 0; i < workers.size(); i++) {
				delete (workers[i]);
			}

			workers.clear();
		}

		//count; collect; sum; flat
		template< typename Param1 = NULL_TYPE, typename Param2 = NULL_TYPE >
		void createOperators(int const& no_workers){
			for(int i = 0; i < no_workers; i++){
				workers.push_back(new TOperator());
			}
		}

		//map
		template< typename Param1, typename Param2 >
		void createOperators(int const& no_workers, std::function< Param2*(Param1*) > const& taskFunc){
			for(int i = 0; i < no_workers; i++){
				workers.push_back(new TOperator(taskFunc));
			}
		}

		//find
		template< typename Param1, typename Param2 >
		void createOperators(int const& no_workers, std::function< Param2(Param1*) > const& taskFunc){
			for(int i = 0; i < no_workers; i++){
				workers.push_back(new TOperator(taskFunc));
			}
		}

		//peek
		template< typename Param1, typename Param2 >
		void createOperators(int const& no_workers, std::function< void(Param1*) > const& taskFunc){
			for(int i = 0; i < no_workers; i++){
				workers.push_back(new TOperator(taskFunc));
			}
		}

		//source
		template< typename Param1, typename Param2 >
		void createOperators(int const& no_workers, Param1 &param1, Param2 &param2){
			for(int i = 0; i < no_workers; i++){
				workers.push_back(new TOperator(param1, param2));
			}
		}
	};
}

#endif
