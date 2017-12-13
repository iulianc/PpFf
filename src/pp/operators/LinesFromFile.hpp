
#include <ff/node.hpp>
#include <string>
#include <sstream>
#include <iostream>

using namespace ff;

class LinesFromFile: public ff_node {
public:
    LinesFromFile(const std::string& path) : path(path) {
    }

    ~LinesFromFile() {
    }

    void* svc(void* task) {
        std::ifstream file(path);

        for  (std::string line; std::getline(file, line); ) {
            this->ff_send_out( new std::string(line) );
        }
        return EOS;
    }

private:
    const std::string& path;
};
