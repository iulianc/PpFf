
#include <ff/node.hpp>
#include "../utilities/NullType.hpp"
#include <functional>

template < typename In, typename K, typename V, bool IsTaskFunc, bool IsBinaryOperator >
class GroupByKey: public ff_node {};

template < typename In, typename K, typename V >
class GroupByKey< In, K, V, true, true >: public ff_node {
public:
	GroupByKey(std::map < K, V > &container, std::function< K(In) > const& taskf, V identity, std::function< V(V, In) > const& binaryOperator): container(container), taskf(taskf), identity(identity), binaryOperator(binaryOperator){};
	~GroupByKey(){};

	void* svc(void* task) {
		In *in = (In*)task;
		K *key = new K(taskf(*(In*)task));

		if(container.find(*key) != container.end()){
			container[*key] = binaryOperator(container[*key], *in);
		}
		else{
			//container[*key] = VALUE();
			container[*key] = binaryOperator(identity, *in);
		}

		return GO_ON;
	}

private:
	typedef V VALUE;
	typedef std::map< K, V > CONTAINER;
	CONTAINER &container;
	std::function< K(In) > const taskf;
	VALUE identity;
	std::function< V(V, In) > const binaryOperator;
};


template < typename In, typename K, typename V >
class GroupByKey< In, K, V, false, true >: public ff_node {
public:
	GroupByKey(std::map < K, V > &container, V identity, std::function< V(V, In) > const& binaryOperator): container(container), identity(identity), binaryOperator(binaryOperator){};
	~GroupByKey(){};

	void* svc(void* task) {
		In *in = (In*)task;

		if(container.find(*in) != container.end()){
			container[*in] = binaryOperator(container[*in], *in);
		}
		else{
			container[*in] = binaryOperator(identity, *in);
		}

		return GO_ON;
	}

private:
	typedef V VALUE;
	typedef std::map< K, V > CONTAINER;
	CONTAINER &container;
	VALUE identity;
	std::function< V(V, In) > const binaryOperator;
};


template < typename In, typename K, typename V >
class GroupByKey< In, K, V, true, false >: public ff_node {
public:
	GroupByKey(std::map < K , std::vector< V > > &container, std::function< K(In) > const& taskf): container(container), taskf(taskf){};
	~GroupByKey(){};

	void* svc(void *task) {
		In *in = (In*)task;
		K *key = new K(taskf(*(In*)task));

		if(container.find(*key) != container.end()){
			container[*key].push_back(*in);
		}
		else{
			container[*key] = VALUE();
			container[*key].push_back(*in);
		}

		return GO_ON;
	}

private:
	typedef std::vector< V > VALUE;
	typedef std::map< K, VALUE > CONTAINER;
	CONTAINER &container;
	std::function< K(In) > const taskf;
};


template < typename In, typename K, typename V >
class GroupByKey< In, K, V, false, false >: public ff_node {
public:
	GroupByKey(std::map < In , std::vector< In > > &container): container(container){}
	~GroupByKey(){};

	void* svc(void *task) {
		In *in = (In*)task;

		if(container.find(*in) != container.end()){
			container[*in].push_back(*in);
		}
		else{
			//container.insert(CONTAINER::value_type(*in, VALUE()));
			container[*in] = VALUE();
			container[*in].push_back(*in);
		}

		return GO_ON;
	}

private:
	typedef std::vector< In > VALUE;
	typedef std::map< In, VALUE > CONTAINER;
	CONTAINER &container;
};


/*
template < typename In, typename K, typename V, typename TaskFunc, typename BinaryOperator >
class GroupByKey: public ff_node {
public:
	GroupByKey(std::map < K, V > &container, TaskFunc const& taskf, V identity, BinaryOperator const& binaryOperator): container(container), taskf(taskf), identity(identity), binaryOperator(binaryOperator){};
	~GroupByKey(){};

	void* svc(void* task) {
		In *in = (In*)task;
		K *key = new K(taskf(*(In*)task));

		if(container.find(*key) != container.end()){
			container[*key] = binaryOperator(container[*key], *in);
		}
		else{
			//container[*key] = VALUE();
			container[*key] = binaryOperator(identity, *in);
		}

		return GO_ON;
	}

private:
	typedef V VALUE;
	typedef std::map< K, V > CONTAINER;
	CONTAINER &container;
	TaskFunc const taskf;
	VALUE identity;
	BinaryOperator const binaryOperator;
};


template < typename In, typename K, typename V, class TaskFunc >
class GroupByKey< In, K, V, TaskFunc, NULL_TYPE >: public ff_node {
public:
	GroupByKey(std::map < K , std::vector< V > > &container, TaskFunc const& taskf): container(container), taskf(taskf){};
	~GroupByKey(){};

	void* svc(void *task) {
		In *in = (In*)task;
		K *key = new K(taskf(*(In*)task));

		if(container.find(*key) != container.end()){
			container[*key].push_back(*in);
		}
		else{
			container[*key] = VALUE();
			container[*key].push_back(*in);
		}

		return GO_ON;
	}

private:
	typedef std::vector< V > VALUE;
	typedef std::map< K, VALUE > CONTAINER;
	CONTAINER &container;
	TaskFunc const taskf;
};

template < typename In, typename K, typename V >
class GroupByKey< In, K, V, NULL_TYPE, NULL_TYPE >: public ff_node {
public:
	GroupByKey(std::map < In , std::vector< In > > &container): container(container){}
	~GroupByKey(){};

	void* svc(void *task) {
		In *in = (In*)task;

		if(container.find(*in) != container.end()){
			container[*in].push_back(*in);
		}
		else{
			//container.insert(CONTAINER::value_type(*in, VALUE()));
			container[*in] = VALUE();
			container[*in].push_back(*in);
		}

		return GO_ON;
	}

private:
	typedef std::vector< In > VALUE;
	typedef std::map< In, VALUE > CONTAINER;
	CONTAINER &container;
};
*/
