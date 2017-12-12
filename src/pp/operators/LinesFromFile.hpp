
#include <ff/node.hpp>
#include <string>
#include <sstream>
#include <iostream>

using namespace ff;

class LinesFromFile: public ff_node{
public:
    LinesFromFile(const std::string& path) {
        file.open(path, std::ifstream::in);
    }

    ~LinesFromFile() {
    }

    void* svc(void* task) {
        std::string line;
        if (std::getline(file, line)) {
            std::cout << line << std::endl;
            //this->ff_send_out((void*) line);
            return GO_ON;
        } else {
            return EOS;
        }
    }

private:
    std::ifstream file;
};
