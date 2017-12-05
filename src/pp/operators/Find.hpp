
#include <ff/node.hpp>
#include <functional>

using namespace ff;

template < typename T >
class Find: public ff_node_t< T > {
public:
	Find(std::function< bool(T*) > const& taskFunc): taskFunc(taskFunc){};
	~Find(){};

	T* svc(T* task) {
		if(!taskFunc(task)){
			delete(task);
			return (T*)GO_ON;
		}else{
			return task;
		}
	}


private:
	std::function< bool(T*) > const& taskFunc;
};
