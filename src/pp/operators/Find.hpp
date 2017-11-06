
#include <ff/node.hpp>
#include <functional>

using namespace ff;

template < typename T >
class Find: public ff_node_t< T > {
public:
	Find(std::function< bool(T) > const& taskFunc): taskFunc(taskFunc){};
	~Find(){};

	T* svc(T* task) {
		if(!taskFunc(*task)){
			return (T*)GO_ON;
		}else{
			//std::cout << "Find Stage - Finded item: " << (*task).toString() << "\n";
			return task;
		}
	}


private:
	std::function< bool(T) > const taskFunc;
};
