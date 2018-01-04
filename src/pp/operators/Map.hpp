
#include <ff/node.hpp>
#include <functional>

using namespace ff;

template < typename In, typename Out >
class Map: public ff_node {
public:
	Map(std::function< Out*(In*) > const& taskFunc): taskFunc(taskFunc){};
	~Map(){};

	void* svc(void* task) {
		return taskFunc((In*)task);
	}

private:
	std::function< Out*(In*) > const& taskFunc;
};
