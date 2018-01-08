
#ifndef COLLECTORS_HPP
#define COLLECTORS_HPP

#include "Container.hpp"
#include <utility>
#include <functional>


//enum CollectorType {UNDEFINED, AS_SUM, AS_COUNT, TO_VECTOR, TO_ARRAY};

template <typename T, class C = Container< T, std::vector > >
class Collectors {
public:
	Collectors() {};

	Collectors(int const no_workers) {
		for(int i = 0; i < no_workers; i++){
			T elem{};
			this->container.push(elem);
		}
	};

	C& getContainer(){
		return this->container;
	}

//	void reduce(std::function< void(T&, T&) > const& taskFunc){
//		if(container.size() > 0){
//			for(unsigned int i = 1; i < container.size(); i++){
//		    	taskFunc(container[0], container[i]);
//		    }
//		}
//	}

	void reduce(std::function< void(T*, T*) > const& taskFunc){
		if(container.size() > 0){
			for(unsigned int i = 1; i < container.size(); i++){
		    	taskFunc(&container[0], &container[i]);
		    }
		}
	}

	template <class U = C, typename std::enable_if< ! std::is_same< U, T >::value>::type * = nullptr > U& value(){
		return container;
	}

	template <class U, typename std::enable_if< std::is_same< U, T >::value>::type * = nullptr  > U& value(){
		return container[0];
	}


private:
	C container;
	T item;
};


#endif



