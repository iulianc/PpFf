#ifndef ANYMACHOPERATOR_HPP
#define ANYMACHOPERATOR_HPP

#include <operators/FinalOperator.hpp>
#include <functional>


namespace PpFf{

	template < typename T >
	class AnyMachOperator: public FinalOperator {
	public:
		typedef bool Value;
		AnyMachOperator(std::function< bool(T*) > const& taskFunc): taskFunc(taskFunc) { };
		AnyMachOperator(const AnyMachOperator& other) : taskFunc(other.taskFunc) { }
		AnyMachOperator(AnyMachOperator&& other) noexcept : taskFunc(std::move(other.taskFunc)) { }
		AnyMachOperator& operator+= ( const AnyMachOperator& other ) {
			if(!val){
				val = other.val;
			}
			return *this;
		}
		~AnyMachOperator() { }

		void* svc(void* task) {
			if(val)
				return EOS;

			val = taskFunc((T*)task);

			return (T*)GO_ON;
		}

		bool value(){
			return val;
		}


	private:
		std::function< bool(T*) > const& taskFunc;
		bool val = false;
	};

}

#endif
