#ifndef REDUCEOPERATOR_HPP
#define REDUCEOPERATOR_HPP

#include <operators/FinalOperator.hpp>
#include <operators/Reducing.hpp>


namespace PpFf{

	template < typename In, typename Out >
	class ReduceOperator: public FinalOperator {
	public:
		typedef Out Value;
		ReduceOperator(Reducing< In, Out > const& reducing): reducing(reducing) { };
		ReduceOperator(const ReduceOperator& other) : reducing(other.reducing) { }
		ReduceOperator(ReduceOperator&& other) noexcept : reducing(std::move(other.reducing)) { }
		ReduceOperator& operator+= ( ReduceOperator& other ) {
			if(reducing.isCombiner){
				reducing.combiner(&val, &other.val);
			}
			return *this ;
		}
		~ReduceOperator() { };

		void* svc(void* task) {
			reducing.accumulator(&val, (In*)task);

			return (Out*)GO_ON;
		}

		Out value(){
			return val;
		}

	private:
		Reducing< In, Out > const& reducing;
		Out val = reducing.identity;
	};

}

#endif
