
#include <ff/node.hpp>

using namespace ff;

template < typename In, typename Out, typename TaskFunc >
class Map: public ff_node {
public:
	Map(TaskFunc const& taskf): taskf(taskf){};
	~Map(){};

	void* svc(void* task) {
		return new Out(taskf(*(In*)task));
	}

private:
	TaskFunc const taskf;
};


//template < typename In, typename Out, typename TaskFunc >
//class Map< In, Out*, TaskFunc >: public ff_node {
//public:
//	Map(TaskFunc const& taskf): taskf(taskf){};
//	~Map(){};
//
//	void* svc(void* task) {
//		Out *out = taskf(*(In*)task);
//		return &out;
//	}
//
//	template<typename T>
//	T * ptr(T obj) {
//		T *result = &obj;
//		return result;
//	}
//
//	template<typename T>
//	T * ptr(T *obj) {
//		//T *result = new T;
//		T *result;
//		result = obj;
//		return result;
//	}
//
//private:
//	TaskFunc const taskf;
//};
