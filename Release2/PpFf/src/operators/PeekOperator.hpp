#ifndef PEEKOPERATOR_HPP
#define PEEKOPERATOR_HPP

#include <functional>
#include <operators/BaseOperator.hpp>


namespace PpFf{

	template < typename T >
	class PeekOperator: public BaseOperator {
	public:
		PeekOperator(std::function< void(T*) > const& taskFunc): taskFunc(taskFunc) { }
		PeekOperator(const PeekOperator& other) : taskFunc(other.taskFunc) { }
		PeekOperator(PeekOperator&& other) noexcept : taskFunc(std::move(other.taskFunc)) { }
		~PeekOperator() { }

		void* svc(void* task) {
			taskFunc((T*)task);
			return task;
		}

	private:
		std::function< void(T*) > const &taskFunc;
	};

}

#endif
