#ifndef LINEFROMFILEOPERATOR_HPP
#define LINEFROMFILEOPERATOR_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <operators/BaseOperator.hpp>

using namespace ff;

namespace PpFf {
    
    class LinesFromFileOperator: public BaseOperator {
    public:
        LinesFromFileOperator(const std::string& path) : path(path)
        {}
        LinesFromFileOperator(const LinesFromFileOperator& other) : path(other.path)
        {}
        LinesFromFileOperator(LinesFromFileOperator&& other) noexcept : path(std::move(other.path))
        {}

        ~LinesFromFileOperator()
        {}

        void* svc(void* task) {
            std::ifstream file(path);

            std::string* line = new std::string;
            while (std::getline(file, *line)) {
                ff_send_out(line);
                line = new std::string;
            }
            file.close();
            return EOS;
        }

    private:
        const std::string& path;
    };

}

#endif
