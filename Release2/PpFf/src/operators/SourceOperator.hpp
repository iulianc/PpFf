#ifndef SOURCEOPERATOR_HPP
#define SOURCEOPERATOR_HPP

#include <ff/node.hpp>
#include <functional>
#include <operators/BaseOperator.hpp>

using namespace ff;

namespace PpFf {

    template< typename T, typename It, bool Preserve = false >
    class SourceOperator: public BaseOperator {};

    template< typename T, typename It >
    class SourceOperator< T, It, false >: public BaseOperator {
    public:
        SourceOperator(It &begin, It &end): begin(begin), end(end) {}

        SourceOperator(const SourceOperator& other): begin(other.begin), end(other.end) {}

        SourceOperator(SourceOperator&& other) noexcept: begin(std::move(other.begin)), end(std::move(other.end)) {}

        virtual ~SourceOperator() {}

        void* svc(void* task) {
            for (; begin != end; begin++) {
                this->ff_send_out(new T(*begin));
            }

            return EOS;
        }

    private:
        It begin;
        It end;
    };


    template< typename T, typename It >
    class SourceOperator< T*, It, false >: public BaseOperator {
    public:
        SourceOperator(It const& begin, It const& end): begin(begin), end(end){}

        SourceOperator(const SourceOperator& other): begin(other.begin), end(other.end) {}

        SourceOperator(SourceOperator&& other) noexcept: begin(std::move(other.begin)), end(std::move(other.end)) {}

        ~SourceOperator() {}

        void* svc(void* task) {
            for (; begin != end; begin++) {
                this->ff_send_out(&(*begin));
            }

            return EOS;
        }

    private:
        It begin;
        It end;
    };







//
//    template< typename T, typename It >
//    class SourceOperator< T, It, true >: public BaseOperator {
//    public:
//        SourceOperator(It &_begin, It &_end) {
//            for (; _begin != _end; _begin++) {
//                container.push_back(*_begin);
//            }
//        }
//
//        SourceOperator(const SourceOperator& other): container(other.container) {}
//
//        SourceOperator(SourceOperator&& other) noexcept: container(std::move(other.container)) {}
//
//        virtual ~SourceOperator() {}
//
//        void* svc(void* task) {
//            for(T &val: container){
//                this->ff_send_out (&val);
//            }
//
//            return EOS;
//        }
//
//    private:
//        std::vector<T> container {};
//    };
//
//
//    template< typename T, typename It >
//    class SourceOperator< T*, It, true >: public BaseOperator {
//    public:
//        SourceOperator(It &_begin, It &_end) {
//            for (; _begin != _end; _begin++) {
//                container.push_back(*_begin);
//            }
//        }
//
//        SourceOperator(const SourceOperator& other): container(other.container) {}
//
//        SourceOperator(SourceOperator&& other) noexcept: container(std::move(other.container)) {}
//
//        ~SourceOperator() {}
//
//        void* svc(void* task) {
//            for(T* val: container){
//                this->ff_send_out (val);
//            }
//
//            return EOS;
//        }
//
//    private:
//        std::vector< T* > container {};
//    };




    //    template< typename T, typename It >
    //    class SourceOperator: public BaseOperator {
    //    public:
    //        SourceOperator(It const& begin, It const& end): begin(begin), end(end) {}
    //
    //        SourceOperator(const SourceOperator& other): begin(other.begin), end(other.end) {}
    //
    //        SourceOperator(SourceOperator&& other) noexcept: begin(std::move(other.begin)), end(std::move(other.end)) {}
    //
    //        virtual ~SourceOperator() {}
    //
    //        void* svc(void* task) {
    //            for (; begin != end; begin++) {
    //                this->ff_send_out(new T(*begin));
    //            }
    //
    //            return EOS;
    //        }
    //
    //    private:
    //        It begin;
    //        It end;
    //    };
    //
    //
    //    template< typename T, typename It >
    //    class SourceOperator<T*, It>: public BaseOperator {
    //    public:
    //        SourceOperator(It const& begin, It const& end): begin(begin), end(end){}
    //
    //        SourceOperator(const SourceOperator& other): begin(other.begin), end(other.end) {}
    //
    //        SourceOperator(SourceOperator&& other) noexcept: begin(std::move(other.begin)), end(std::move(other.end)) {}
    //
    //        ~SourceOperator() {}
    //
    //        void* svc(void* task) {
    //            for (; begin != end; begin++) {
    //                this->ff_send_out(&(*begin));
    //            }
    //
    //            return EOS;
    //        }
    //
    //    private:
    //        It begin;
    //        It end;
    //    };

}

#endif
