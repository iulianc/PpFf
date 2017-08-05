
#ifndef ACCUMULATOR_HPP
#define ACCUMULATOR_HPP

#include <vector>


template< typename T >
class AccumulationTraits;

template<>
class AccumulationTraits< char > {
public:
	typedef int AccT;
	static AccT init() {
		return 0;
	}
};

template<>
class AccumulationTraits< short > {
public:
	typedef int AccT;
	static AccT init(){
		return 0;
	}
};

template<>
class AccumulationTraits< int >{
public:
	typedef long AccT;
	static AccT init(){
		return 0;
	}
};

template<>
class AccumulationTraits< unsigned int > {
  public:
	typedef unsigned long AccT;
	static AccT init() {
		return 0;
	}
};

template<>
class AccumulationTraits< float > {
  public:
	typedef double AccT;
	static AccT init() {
		return 0;
	}
};

template<>
class AccumulationTraits< double > {
  public:
	typedef double AccT;
	static AccT init() {
		return 0;
	}
};

template < typename T, typename Traits >
class Accumulator;

template < typename T, typename Traits = AccumulationTraits<T> >
class Accumulator {
public:

	Accumulator(){
		total = Traits::init();
	};

	Accumulator< T, Traits > operator+(const Accumulator< T, Traits > &acc){
		Accumulator< T, Traits > temp;
		temp.total = total + acc.total;
		return temp;
	}

	Accumulator< T, Traits > &operator+=(const Accumulator< T, Traits > &acc){
		total += acc.total;
		return this;
	}

	bool isAccumulator(){
		return true;
	}

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

#endif



