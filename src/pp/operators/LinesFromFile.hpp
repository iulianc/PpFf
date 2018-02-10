#include <ff/node.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace ff;

class LinesFromFile: public ff_node {
public:
    LinesFromFile(const std::string& path) : path(path) {}
    ~LinesFromFile() {}

    void* svc(void* task) {
        std::ifstream file(path);

        std::string* line = new std::string;
        while (std::getline(file, *line)) {
            ff_send_out(line);
            line = new std::string;
        }
        return EOS;
    }

private:
    const std::string& path;
};
