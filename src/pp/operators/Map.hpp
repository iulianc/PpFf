
#include <ff/node.hpp>
#include <functional>

using namespace ff;

template < typename In, typename Out >
class Map: public ff_node {
public:
	Map(std::function< Out(In) > const& taskFunc): taskFunc(taskFunc){};
	~Map(){};

	void* svc(void* task) {
//		Out *out = new Out;
//		*out = taskf(*(In*)task);
		return new Out(taskFunc(*(In*)task));
		//return out;
	}

private:
	std::function< Out(In) > taskFunc;
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
