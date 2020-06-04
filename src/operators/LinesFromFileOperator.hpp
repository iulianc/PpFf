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

    class LinesFromFileOperator_: public BaseOperator {
    public:
        LinesFromFileOperator_(const std::string& path, const int blockSize) : path(path), blockSize(blockSize)
        {}
        LinesFromFileOperator_(const LinesFromFileOperator_& other) : path(other.path), blockSize(other.blockSize)
        {}
        LinesFromFileOperator_(LinesFromFileOperator_&& other) noexcept : path(std::move(other.path)), blockSize(other.blockSize)
        {}

        ~LinesFromFileOperator_()
        {}

        void* svc(void* task) {
            std::ifstream file(path);

            //std::cerr << "** blockSize = " << blockSize << std::endl;

            std::string* strings = new std::string[blockSize];

            int next = 0;
            while (std::getline(file, strings[next])) {
                ff_send_out(&strings[next]);
                next += 1;
                if (next == blockSize) {
                    strings = new std::string[blockSize];
                    next = 0;
                }
            }
            file.close();
            return EOS;
        }

    private:
        const std::string& path;
        int blockSize;
    };

}

#endif
