
#ifndef THREADMANAGER_HPP
#define THREADMANAGER_HPP

#include "Container.hpp"
#include <utility>
#include <functional>



template <typename T, class C = Container< T, std::vector > >
class Workers {
public:
	typedef C Container;

//	Workers(){};

//	Workers(int const no_workers) {
//		for(int i = 0; i < no_workers; i++){
//			T* elem = new T();
//			this->container.push(elem);
//		}
//	};

	C& getContainer(){
		return this->container;
	}

	void push (T const& elem){
		container.push(elem);
	}

	void reduce(std::function< void(T&, T&) > const& taskFunc){
		if(container.size() > 0){
			for(unsigned int i = 1; i < container.size(); i++){
				taskFunc(container[0], container[i]);
			}
		}
	}

//	void reduce(std::function< void(T*, T*) > const& taskFunc){
//		if(container.size() > 0){
//			for(unsigned int i = 1; i < container.size(); i++){
//		    	taskFunc(container[0], container[i]);
//		    }
//		}
//	}


	T& value(){
		return container[0];
	}


private:
	Container container;
};


#endif



