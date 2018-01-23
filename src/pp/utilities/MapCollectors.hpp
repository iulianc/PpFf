
#ifndef MAPCOLLECTORS_HPP
#define MAPCOLLECTORS_HPP

#include "Container.hpp"
#include <utility>
#include <functional>
#include "../aggregates/Aggregate.hpp"


template <typename K, typename V = K, class C = Container< V, std::vector > >
class MapCollectors {
public:
	typedef K Key;
	typedef V Value;
    typedef C Container;


	V& value(){
		return this->val;
	}

	C& container(){
		return this->cont;
	};

//	static Aggregate< 0, V > const& count(){
//		static Aggregate< 0, V > aggregateOperatorCount;
//		return aggregateOperatorCount;
//	}
//
//	static Aggregate< 1, V > const& sum(){
//		static Aggregate< 1, V > aggregateOperatorSum;
//		return aggregateOperatorSum;
//	}

	void printContainer(){
		for (auto it = cont.begin(); it != cont.end(); it++) {
			std::cout << it->first << "-" << it->second << std::endl;
		}
	}

	void printValue(){
		std::cout << val << std::endl;
	}

private:
	Key key{};
	Value val{};
	Container cont{};
};


#endif



