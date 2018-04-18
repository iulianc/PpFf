#ifndef IDENTITY_HPP
#define IDENTITY_HPP

namespace PpFf {

    template< typename In, typename Out >
    Out* identity( In* x ) {
        return (Out*) x;
    }

}

#endif
