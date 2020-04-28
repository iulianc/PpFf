#ifndef DEBUG_HPP
#define DEBUG_HPP

#define _DEBUG_ 1
//#define _DEBUG_ 0

namespace PpFf {

    class Debug {
    public:
        static void debug( std::string s ) {
            if (_DEBUG_)
                printf( "[DEBUG] %s\n", s.c_str() );
        }
    };

}

#endif
