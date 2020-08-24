
#ifndef COLLECTION_HPP
#define COLLECTION_HPP

#include <stdexcept>
#include <memory>
#include <utilities/NullType.hpp>

using namespace PpFf;

namespace PpFf {

    template < typename T,
               template <typename ELEM, class ALLOC = std::allocator<ELEM>>
               class CONT, 
               typename P = NULL_TYPE >

    class Collection {
    public:
        typedef CONT<T> Container;
        typedef typename CONT<T>::iterator iterator;

        Collection() {};

        Collection(Container && container) : container(std::move(container)) {};
        
        Collection(unsigned int n) {
            container.resize(n);
        };

        ~Collection() {};
        
        T& operator[] (unsigned int i) {
            if (i >= container.size()) {
                throw std::out_of_range("error:operator[]");
            }

            return container[i];
        }

        template< typename T2,
                  template< typename ELEM2, 
                            typename = std::allocator<ELEM2>> class CONT2, 
                  typename P2 >
        Collection<T, CONT, P>& operator= (Collection<T2, CONT2, P2> const& op2) {
            if ((void*) this == (void*) &op2) {
                return *this;
            }

            Collection<T2, CONT2, P2> tmp(op2);
            container.clear();
            while (!tmp.empty()) {
                container.push_front(tmp.top());
                tmp.pop();
            }

            return *this;
        }

        iterator begin() {
            return container.begin();
        }

        iterator end() {
            return container.end();
        }

        void push_back(T const& elem) {
            container.push_back(elem);
        }

        T back() const {
            if (container.empty()) {
                throw std::out_of_range("Collection<>::back(): empty Container");
            }

            return container.back();
        }

        T at(unsigned int i) {
            if (i >= container.size()) {
                throw std::out_of_range("error:Collection<>::at()");
            }

            return container.at(i);
        }

        Container value() {
            return container;
        }

        unsigned int size() {
            return container.size();
        }

        void pop_back() {
            if (container.empty()) {
                throw std::out_of_range("Collection<>::pop_back(): empty Container");
            }
            container.pop_back();
        }

        bool empty() const {
            return container.empty();
        }


        T top() const {
            return container.empty();
        }

        void insert(iterator beginInsert, iterator beginContainer, iterator endContainer) {
            container.insert(beginInsert, beginContainer, endContainer);
        }

        P& stream() {
            return pipe.template source<T>(container.begin(), container.end());
        }

    private:
        Container container;
        P pipe{};
    };

}


#endif
