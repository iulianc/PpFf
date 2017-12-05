
#include <ff/node.hpp>
#include "../utilities/NullType.hpp"
#include <functional>

template < typename In, typename K, typename V, bool IsTaskFunc, bool IsBinaryOperator >
class GroupByKey: public ff_node {};


template < typename In, typename K, typename V >
class GroupByKey< In, K, V, true, true >: public ff_node {
public:
	GroupByKey(std::map < K, V > &container, std::function< K*(In*) > const &taskf, std::function< void(V&, In*) > const& binaryOperator): container(container), taskf(taskf), binaryOperator(binaryOperator){};
	~GroupByKey(){};


	void* svc(void* task) {
		K *key = taskf((In*)task);
		binaryOperator(container[*key], (In*)task);

		return GO_ON;
	}

private:
	typedef std::map< K, V > CONTAINER;
	CONTAINER &container;
	std::function< K*(In*) > const &taskf;
	std::function< void(V&, In*) > const &binaryOperator;
};



template < typename In, typename K, typename V >
class GroupByKey< In, K, V, false, true >: public ff_node {
public:
	GroupByKey(std::map < K, V > &container, std::function< void(V&, In*) > const& binaryOperator): container(container), binaryOperator(binaryOperator){};
	~GroupByKey(){};

	void* svc(void* task) {
		binaryOperator(container[*((In*)task)], (In*)task);

		return GO_ON;
	}

private:
	typedef std::map< K, V > CONTAINER;
	CONTAINER &container;
	std::function< void(V&, In*) > const binaryOperator;
};



template < typename In, typename K, typename V >
class GroupByKey< In, K, V, true, false >: public ff_node {
public:
	GroupByKey(std::map < K , std::vector< V > > &container, std::function< K*(In*) > const& taskf): container(container), taskf(taskf){};
	~GroupByKey(){};

	void* svc(void *task) {
		K *key = taskf((In*)task);

//		if(container.find(*key) != container.end()){
//			container[*key].push_back(*((In*)task));
//		}
//		else{
//			container[*key] = VALUE();
//			container[*key].push_back(*((In*)task));
//		}

		container[*key].push_back(*((In*)task));

		return GO_ON;
	}

private:
	typedef std::vector< V > VALUE;
	typedef std::map< K, VALUE > CONTAINER;
	CONTAINER &container;
	std::function< K*(In*) > const &taskf;
};



template < typename In, typename K, typename V >
class GroupByKey< In, K, V, false, false >: public ff_node {
public:
	GroupByKey(std::map < In , std::vector< In > > &container): container(container){}
	~GroupByKey(){};

	void* svc(void *task) {
		In *in = (In*)task;

		container[*in].push_back(*in);

//		if(container.find(*in) != container.end()){
//			container[*in].push_back(*in);
//		}
//		else{
//			//container.insert(CONTAINER::value_type(*in, VALUE()));
//			container[*in] = VALUE();
//			container[*in].push_back(*in);
//		}

		return GO_ON;
	}

private:
	typedef std::vector< In > VALUE;
	typedef std::map< In, VALUE > CONTAINER;
	CONTAINER &container;
};
