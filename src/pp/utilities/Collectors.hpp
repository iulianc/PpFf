
#ifndef COLLECTORS_HPP
#define COLLECTORS_HPP

#include "Container.hpp"
#include <utility>
#include <functional>


enum CollectorType {UNDEFINED, AS_SUM, AS_COUNT, TO_VECTOR, TO_ARRAY};


//template <typename T, class C = Container< T, std::vector > >
template <typename T, typename V = T, class C = Container< T, std::vector > >
class Collectors {
public:
	Collectors(): collector_type(UNDEFINED){
		is_collection = false;
	};

	C& getContainer(){
		return this->container;
	}


	void collect(CollectorType collector_type){
		this->collector_type = collector_type;
	}

	template <class U = C, typename std::enable_if< ! std::is_same< U, T >::value>::type * = nullptr > U value(){
		return container;
	}

	template <class U, typename std::enable_if< std::is_same< U, T >::value>::type * = nullptr  > U value(){
		T result;

		for(unsigned int i = 0; i < container.size(); i++){
			reduce(container.at(i), result, ([](T in, T &out){out = out + in;}));
		}

		return result;
	}


private:
	C container;
	T item;
	bool is_collection;

	CollectorType collector_type;

//	typedef void (*FuncTask)(T, T&);
//	void reduce(T in, T &out, FuncTask funcTask){
//		funcTask(in, out);
//	}

	void reduce(T in, T &out, std::function< void(T, T&) > const& taskFunc){
		taskFunc(in, out);
	}


};


#endif



