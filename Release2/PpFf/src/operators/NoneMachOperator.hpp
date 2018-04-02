#ifndef NONEMACHOPERATOR_HPP
#define NONEMACHOPERATOR_HPP

#include <operators/FinalOperator.hpp>
#include <functional>


namespace PpFf{

	template < typename T >
	class NoneMachOperator: public FinalOperator {
	public:
		typedef bool Value;
		NoneMachOperator(std::function< bool(T*) > const& taskFunc): taskFunc(taskFunc) { };
		NoneMachOperator(const NoneMachOperator& other) : taskFunc(other.taskFunc) { }
		NoneMachOperator(NoneMachOperator&& other) noexcept : taskFunc(std::move(other.taskFunc)) { }
		NoneMachOperator& operator+= ( const NoneMachOperator& other ) {
			if(val){
				val = other.val;
			}
			return *this;
		}
		~NoneMachOperator() { }

		void* svc(void* task) {
			if(!val)
				return EOS;

			val = !(taskFunc((T*)task));

			return (T*)GO_ON;
		}

		bool value(){
			return val;
		}


	private:
		std::function< bool(T*) > const& taskFunc;
		bool val = true;
	};

}

#endif
