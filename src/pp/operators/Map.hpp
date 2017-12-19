
#include <ff/node.hpp>
#include <functional>

using namespace ff;

template < typename In, typename Out >
class Map: public ff_node {
public:
    Map(std::function< Out*(In*) > const& taskFunc): taskFunc(taskFunc){};
    ~Map(){};
    
    int svc_init() {
        myTimer = startTimer();
        
        return 0;
    }
    
    void* svc(void* task) {
        return taskFunc((In*)task);
    }

    void svc_end() {
        long duration_ms = stopTimer(myTimer);
        
        if (displayTimer())
            std::cerr << "Map[" << myTimer << "]: " << duration_ms << " ms" << std::endl;
    }

private:
    std::function< Out*(In*) > const& taskFunc;
    int myTimer;
};
