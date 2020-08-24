#ifndef LINEFROMFILEOPERATOR_HPP
#define LINEFROMFILEOPERATOR_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <ff/ff.hpp>
#include <operators/Operator.hpp>

namespace PpFf {
    
    class LinesFromFileOperator: public Operator {
    public:
        LinesFromFileOperator(const std::string& path) : path(path)
        { operatorType = SOURCE_OP; }

        LinesFromFileOperator(const LinesFromFileOperator& other) : path(other.path)
        { operatorType = SOURCE_OP; }

        LinesFromFileOperator(LinesFromFileOperator&& other) noexcept : path(std::move(other.path))
        { operatorType = SOURCE_OP; }

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
