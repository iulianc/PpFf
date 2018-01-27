
#include <ff/node.hpp>
#include "../utilities/NullType.hpp"
#include "../utilities/MapType.hpp"
#include "../aggregates/Aggregate.hpp"
#include <functional>



template < int I, typename In, typename K, typename V, typename C, typename Aggr >
class GroupByKey: public ff_node {};


template < typename In, typename K, typename V, typename C, typename Aggr >
class GroupByKey< 0, In, K, V, C, Aggr >: public ff_node {
public:
	typedef typename C::Container Container;

	GroupByKey(C* collectors, Aggr &aggregate): collectors(collectors), aggregate(aggregate){};
	~GroupByKey(){};

	void* svc(void* task) {
		aggregate.compute((&container[*((In*)task)]), (In*)task);

		return GO_ON;
	}

private:
	C *collectors;
	Aggr &aggregate;

	Container &container = collectors->container();
};

template < typename In, typename K, typename V, typename C, typename Aggr >
class GroupByKey< 1, In, K, V, C, Aggr >: public ff_node {
public:
	typedef typename C::Container Container;


	GroupByKey(C* collectors, std::function< K*(In*) > const& taskFuncOnKey, Aggr &aggregate): collectors(collectors), taskFuncOnKey(taskFuncOnKey), aggregate(aggregate){};
	~GroupByKey(){};

	void* svc(void* task) {
		K *key = taskFuncOnKey((In*)task);
		aggregate.compute((&container[*key]), (In*)task);

		return GO_ON;
	}

private:
	C *collectors;
	std::function< K*(In*) > const& taskFuncOnKey;
	Aggr &aggregate;

	Container &container = collectors->container();
};


template < typename In, typename K, typename V, typename C, typename Aggr >
class GroupByKey< 2, In, K, V, C, Aggr >: public ff_node {
public:
	typedef typename C::Container Container;


	GroupByKey(C* collectors, std::function< K*(In*) > const& taskFuncOnKey, std::function< V*(In*) > const& taskFuncOnValue, Aggr &aggregate): collectors(collectors), taskFuncOnKey(taskFuncOnKey), taskFuncOnValue(taskFuncOnValue), aggregate(aggregate){};
	~GroupByKey(){};

	void* svc(void* task) {
		K *key = taskFuncOnKey((In*)task);
		V *value = taskFuncOnValue((In*)task);
		it = container.find(*key);
		//aggregate.compute((&container[*key]), value);

		if (it != container.end()){
			aggregate.compute(&(it->second), value);
		}else{
			container[*key] = *value;
		}

		return GO_ON;
	}

private:
	C *collectors;
	std::function< K*(In*) > const& taskFuncOnKey;
	std::function< V*(In*) > const& taskFuncOnValue;
	Aggr &aggregate;

	Container &container = collectors->container();

	typedef std::pair < V, int > Value;
	typedef Aggregate< Aggregates::OperatorAvg, V, Value > AggOpAvg;
	typedef MapType < K, V > MapKV;
	typedef MapType < K, Value > MapKPair;
	typedef typename std::conditional< std::is_same< Aggr, AggOpAvg >::value, MapKPair, MapKV >::type Map;

	typename Map::iterator it;
};

//Average
template < typename In, typename K, typename V, typename C, typename Aggr >
class GroupByKey< 3, In, K, V, C, Aggr >: public ff_node {
public:
	typedef typename C::Container Container;


	GroupByKey(C* collectors, std::function< K*(In*) > const& taskFuncOnKey, std::function< V*(In*) > const& taskFuncOnValue, Aggr &aggregate): collectors(collectors), taskFuncOnKey(taskFuncOnKey), taskFuncOnValue(taskFuncOnValue), aggregate(aggregate){};
	~GroupByKey(){};

	void* svc(void* task) {
		K *key = taskFuncOnKey((In*)task);
		V *value = taskFuncOnValue((In*)task);
		aggregate.compute((&container[*key]), value);

		return GO_ON;
	}

private:
	C *collectors;
	std::function< K*(In*) > const& taskFuncOnKey;
	std::function< V*(In*) > const& taskFuncOnValue;
	Aggr &aggregate;

	Container &container = collectors->container();

//	typedef std::pair < V, int > Value;
//	typedef Aggregate< Aggregates::OperatorAvg, V, Value > AggOpAvg;
//	typedef MapType < K, V > MapKV;
//	typedef MapType < K, Value > MapKPair;
//	typedef typename std::conditional< std::is_same< Aggr, AggOpAvg >::value, MapKPair, MapKV >::type Map;
//
//	typename Map::iterator it;
};


template < typename In, typename K, typename V, typename C, typename Aggr >
class GroupByKey< 4, In, K, V, C, Aggr >: public ff_node {
public:
	typedef typename C::Container Container;

	GroupByKey(C* collectors): collectors(collectors){};
	~GroupByKey(){};

	void* svc(void* task) {
		container[*((In*)task)].push_back(*((In*)task));
		return GO_ON;
	}

private:
	C *collectors;
	Container &container = collectors->container();
};


template < typename In, typename K, typename V, typename C, typename Aggr >
class GroupByKey< 5, In, K, V, C, Aggr >: public ff_node {
public:
	typedef typename C::Container Container;

	GroupByKey(C* collectors, std::function< K*(In*) > const& taskFuncOnKey): collectors(collectors), taskFuncOnKey(taskFuncOnKey){};
	~GroupByKey(){};

	void* svc(void* task) {
		K *key = taskFuncOnKey((In*)task);
		container[*key].push_back(*((In*)task));

		return GO_ON;
	}

private:
	C *collectors;
	std::function< K*(In*) > const& taskFuncOnKey;

	Container &container = collectors->container();
};


template < typename In, typename K, typename V, typename C, typename Aggr >
class GroupByKey< 6, In, K, V, C, Aggr >: public ff_node {
public:
	typedef typename C::Container Container;

	GroupByKey(C* collectors, std::function< K*(In*) > const& taskFuncOnKey, std::function< V*(In*) > const& taskFuncOnValue): collectors(collectors), taskFuncOnKey(taskFuncOnKey), taskFuncOnValue(taskFuncOnValue){};
	~GroupByKey(){};

	void* svc(void* task) {
		K *key = taskFuncOnKey((In*)task);
		V *value = taskFuncOnValue((In*)task);
		container[*key].push_back(*value);

		return GO_ON;
	}

private:
	C *collectors;
	std::function< K*(In*) > const& taskFuncOnKey;
	std::function< V*(In*) > const& taskFuncOnValue;

	Container &container = collectors->container();
};



//
////*****GroupByKey-Deprecated**********************************
//template < typename In, typename K, typename V, bool IsTaskFunc, bool IsBinaryOperator >
//class GroupByKey: public ff_node {};
//
//
//template < typename In, typename K, typename V >
//class GroupByKey< In, K, V, true, true >: public ff_node {
//public:
//	GroupByKey(MapType < K, V > &container, std::function< K*(In*) > const &taskf, std::function< void(V&, In*) > const& binaryOperator): container(container), taskf(taskf), binaryOperator(binaryOperator){};
//	~GroupByKey(){};
//
//
//	void* svc(void* task) {
//		K *key = taskf((In*)task);
//		binaryOperator(container[*key], (In*)task);
//
//		return GO_ON;
//	}
//
//private:
//	typedef MapType< K, V > CONTAINER;
//	CONTAINER &container;
//	std::function< K*(In*) > const &taskf;
//	std::function< void(V&, In*) > const &binaryOperator;
//};
//
//
//
//template < typename In, typename K, typename V >
//class GroupByKey< In, K, V, false, true >: public ff_node {
//public:
//	GroupByKey(MapType < K, V > &container, std::function< void(V&, In*) > const& binaryOperator): container(container), binaryOperator(binaryOperator){};
//	~GroupByKey(){};
//
//	void* svc(void* task) {
//		binaryOperator(container[*((In*)task)], (In*)task);
//
//		return GO_ON;
//	}
//
//private:
//	typedef MapType< K, V > CONTAINER;
//	CONTAINER &container;
//	std::function< void(V&, In*) > const binaryOperator;
//};
//
//
//
//template < typename In, typename K, typename V >
//class GroupByKey< In, K, V, true, false >: public ff_node {
//public:
//	GroupByKey(MapType < K , std::vector< V > > &container, std::function< K*(In*) > const& taskf): container(container), taskf(taskf){};
//	~GroupByKey(){};
//
//	void* svc(void *task) {
//		K *key = taskf((In*)task);
//
//		container[*key].push_back(*((In*)task));
//
//		return GO_ON;
//	}
//
//private:
//	typedef std::vector< V > VALUE;
//	typedef MapType< K, VALUE > CONTAINER;
//	CONTAINER &container;
//	std::function< K*(In*) > const &taskf;
//};
//
//
//
//template < typename In, typename K, typename V >
//class GroupByKey< In, K, V, false, false >: public ff_node {
//public:
//	GroupByKey(MapType < In , std::vector< In > > &container): container(container){}
//	~GroupByKey(){};
//
//	void* svc(void *task) {
//		In *in = (In*)task;
//
//		container[*in].push_back(*in);
//
//		return GO_ON;
//	}
//
//private:
//	typedef std::vector< In > VALUE;
//	typedef MapType< In, VALUE > CONTAINER;
//	CONTAINER &container;
//};

