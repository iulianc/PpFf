
#include <ff/node.hpp>

using namespace ff;


	/*
	 * Passing iterator to constructor
	 * https://stackoverflow.com/questions/28020465/passing-iterators-to-class-functions
	 */

template< typename T, typename It >
class Source: public ff_node_t<T> {
public:
	Source(It const& begin, It const& end): begin(begin), end(end){}
	~Source(){}

	T* svc(T* task) {
		while(begin != end){
			this->ff_send_out(new T(*begin));
			begin ++;
		}

		return (T*)EOS;
	}

private:
	It begin;
	It end;
};

template< typename T, typename It >
class Source< T*, It >: public ff_node_t<T> {
public:
	Source(It const& begin, It const& end): begin(begin), end(end){}
	~Source(){}

	T* svc(T* task) {
		while(begin != end){
			this->ff_send_out(&(*begin));
			begin ++;
		}

		return (T*)EOS;
	}

private:
	It begin;
	It end;
};
