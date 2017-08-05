
#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <deque>
#include <stdexcept>
#include <memory>

template <typename T,
          template <typename ELEM,
                    class ALLOC = std::allocator<ELEM> >
                    class CONT >
class Container {
  private:
    CONT<T> elems;

  public:
    void push(T const&);
    void pop();
    T top() const;
    unsigned size();
    T* at(unsigned int);
    void setWorkers(int);
    CONT< T, std::allocator<T> > value();
    bool empty() const {
        return elems.empty();
    }

    template<typename T2,
             template<typename ELEM2,
                      typename = std::allocator<ELEM2>
                     >class CONT2>
    Container<T,CONT>& operator= (Container<T2,CONT2> const&);
};

template <typename T, template <typename,typename> class CONT>
unsigned int Container<T,CONT>::size(){
	return elems.size();
}

template <typename T, template <typename,typename> class CONT>
T* Container<T,CONT>::at(unsigned int i) {
    if (i >= elems.size()) {
        throw std::out_of_range("error:Container<>::at()");
    }

	return &elems.at(i);
}

template <typename T, template <typename,typename> class CONT>
void Container<T,CONT>::push (T const& elem){
    elems.push_back(elem);
}

template<typename T, template <typename,typename> class CONT>
void Container<T,CONT>::pop ()
{
    if (elems.empty()) {
        throw std::out_of_range("Container<>::pop(): empty Container");
    }
    elems.pop_back();
}

template <typename T, template <typename,typename> class CONT>
T Container<T,CONT>::top () const
{
    if (elems.empty()) {
        throw std::out_of_range("Container<>::top(): empty Container");
    }
    return elems.back();
}

template <typename T, template <typename,typename> class CONT>
CONT< T, std::allocator< T > > Container< T, CONT >::value ()
{
    return elems;
}

template <typename T, template <typename,typename> class CONT>
 template <typename T2, template <typename,typename> class CONT2>
Container<T,CONT>&
Container<T,CONT>::operator= (Container<T2,CONT2> const& op2)
{
    if ((void*)this == (void*)&op2) {
        return *this;
    }

    Container<T2,CONT2> tmp(op2);

    elems.clear();
    while (!tmp.empty()) {
        elems.push_front(tmp.top());
        tmp.pop();
    }
    return *this;
}

#endif
