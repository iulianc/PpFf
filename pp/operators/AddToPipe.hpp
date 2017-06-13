
#include <ff/node.hpp>

using namespace ff;

template< typename T, typename It >
class AddToPipe: public ff_node_t<T> {
public:

	/*
	 * Passing iterator to constructor
	 * https://stackoverflow.com/questions/28020465/passing-iterators-to-class-functions
	 */

	AddToPipe(It const& begin, It const& end): begin(begin), end(end){}
	~AddToPipe(){}

	T* svc(T* task) {
		while(begin != end){
			T* result = new T(*begin);
			*result = *begin;

			std::cout<<"AddToPipe Stage - Item added: "<<*result<<"\n";

			this->ff_send_out(new T(*begin));
			begin ++;
			delete result;
		}

		return (T*)EOS;
	}

private:
	It begin;
	It end;
};
