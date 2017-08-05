
#include <ff/node.hpp>

using namespace ff;

template < typename T, typename TaskFunc >
class Find: public ff_node_t<T> {
public:
	Find(TaskFunc const& taskf): taskf(taskf){};
	~Find(){};

	T* svc(T* task) {
		if(!taskf(*task)){
			return (T*)GO_ON;
		}else{
			//std::cout << "Find Stage - Finded item: " << (*task).toString() << "\n";
			return task;
		}
	}


private:
	TaskFunc const taskf;
};
