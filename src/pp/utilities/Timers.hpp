#ifndef TIMERS_HPP
#define TIMERS_HPP

#include <chrono>
#include <cassert>

#define MAX_TIMERS 1024
static int _nextTimer = 0;
static bool _display = true;
static std::chrono::high_resolution_clock::time_point _begins[MAX_TIMERS];

void setDisplayTimer( bool on ) { _display = on; }
bool displayTimer() { return _display; }

int startTimer() {
    assert( _nextTimer < MAX_TIMERS && "*** Trop de timers sont utilises!!" );

    int timer = _nextTimer;
    _nextTimer += 1;
    _begins[timer] = std::chrono::high_resolution_clock::now();

    return timer;    
}

long stopTimer( int timer ) {
    auto end = std::chrono::high_resolution_clock::now();
    auto begin = _begins[timer];

    return std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
}

#endif

/*
class Timers {
public:
    static void init() {
        nextTimer = 0;
        display = true;
    }

    static int startTimer() {
        int timer = this.nextTimer;
        this.nextTimer += 1;
        this.begins[timer] = std::chrono::high_resolution_clock::now();
        return timer;    
    }
}
private:
    static int nextTimer;
    static bool display;
    static std::chrono::high_resolution_clock::time_point begins[100];
};

*/
