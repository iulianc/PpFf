#include <ff/node.hpp>
#include <string>
#include <sstream>
#include <iostream>

using namespace ff;

class LinesFromFile: public ff_node {
public:
    LinesFromFile(const std::string& path) : path(path) {}
    ~LinesFromFile() {}

    void* svc(void* task) {
        std::ifstream file(path);

        std::string* line;
        while (std::getline(file, *(line = new std::string))) {
            ff_send_out(line);
        }
        return EOS;
    }

private:
    const std::string& path;
};
