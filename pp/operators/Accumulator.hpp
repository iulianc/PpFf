
#include <vector>
#pragma once


template< typename T >
class AccumulationTraits;

template<>
class AccumulationTraits<char> {
public:
	typedef int AccT;
	static AccT init() {
		return 0;
	}
};

template<>
class AccumulationTraits<short> {
public:
	typedef int AccT;
	static AccT init(){
		return 0;
	}
};

template<>
class AccumulationTraits<int>{
public:
	typedef long AccT;
	static AccT init(){
		return 0;
	}
};

template<>
class AccumulationTraits<unsigned int> {
  public:
	typedef unsigned long AccT;
	static AccT init() {
		return 0;
	}
};

template<>
class AccumulationTraits<float> {
  public:
	typedef double AccT;
	static AccT init() {
		return 0;
	}
};

//////template < typename T, T Init, typename CollectorType >
////template < typename T >
////class Accumulator;
//
////template < typename T, T Init, typename CollectorType = pp::Collector::Undefined >
//


template < typename T, typename Traits >
class Accumulator;

template < typename T, typename Traits = AccumulationTraits<T> >
class Accumulator {
public:
//	Accumulator1(T init):total(init){};
//	Accumulator(T init, std::vector< Accumulator < T, Init > > accums):total(init){
//		for(unsigned int i = 0; i < acums.size(); i++){
//			total += acums[i].value;
//		}
//	};

	Accumulator(){
		total = Traits::init();
	};
//	Accumulator(Accumulator const& accum): total(accum.total){};
//	Accumulator& operator = (Accumulator const& accum){
//		total = accum.total;
//		return *this;
//	}


	void accumulate(T val){
		total += val;
	}

	T value(){
		return total;
	}

private:
	typedef typename Traits::AccT AccT;
	AccT total;
};





