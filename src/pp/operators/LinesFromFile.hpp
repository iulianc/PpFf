#include <ff/node.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include "../utilities/timers.hpp"

using namespace ff;

class LinesFromFile: public ff_node {
public:
    LinesFromFile(const std::string& path) : path(path) {}
    ~LinesFromFile() {}

    int svc_init() {
        myTimer = startTimer();
        
        return 0;
    }

    void* svc(void* task) {
        std::ifstream file(path);

        std::string* line = new std::string;
        while (std::getline(file, *line)) {
            ff_send_out(line);
            line = new std::string;
        }
        return EOS;
    }

    void svc_end() {
        long duration_ms = stopTimer(myTimer);
        
        std::cerr << "LinesFromFile[" << myTimer << "]: " << duration_ms << " ms" << std::endl;
    }

private:
    const std::string& path;
    int myTimer;
};
