
#include <ff/node.hpp>
#include <functional>
#include <operators/BaseOperator.hpp>

using namespace ff;

namespace PpFf{

	 template< typename T, typename It >
	 class SourceOperator: public BaseOperator {
	 public:
		SourceOperator(It const& begin, It const& end): begin(begin), end(end){}
		SourceOperator(const SourceOperator& other) : begin(other.begin), end(other.end) { }
		SourceOperator(SourceOperator&& other) noexcept : begin(std::move(other.begin)), end(std::move(other.end)) { }
		virtual ~SourceOperator(){}

		void* svc(void* task) {
			while(begin != end){
				this->ff_send_out(new T(*begin));
				begin ++;
			}

			return EOS;
		}

	 private:
		It begin;
		It end;
	 };


	 template< typename T, typename It >
	 class SourceOperator< T*, It >: public BaseOperator {
	 public:
		SourceOperator(It const& begin, It const& end): begin(begin), end(end){}
		SourceOperator(const SourceOperator& other) : begin(other.begin), end(other.end) { }
		SourceOperator(SourceOperator&& other) noexcept : begin(std::move(other.begin)), end(std::move(other.end)) { }
		~SourceOperator(){}

		void* svc(void* task) {
			while(begin != end){
				this->ff_send_out(&(*begin));
				begin ++;
			}

			return EOS;
		}

	 private:
		It begin;
		It end;
	 };

}
