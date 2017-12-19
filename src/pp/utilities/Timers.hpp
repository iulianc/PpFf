#ifndef TIMERS_HPP
#define TIMERS_HPP

#include <chrono>
#include <cassert>
#include <ctime>

#define MAX_TIMERS 1024
static int _nextTimer = 0;
static bool _display = true;
static std::chrono::high_resolution_clock::time_point _begins[MAX_TIMERS];
static std::time_t _startTimes[MAX_TIMERS];

std::mutex timerMutex;

void setDisplayTimer( bool on ) { _display = on; }
bool displayTimer() { return _display; }

int startTimer() {
    assert( _nextTimer < MAX_TIMERS && "*** Trop de timers sont utilises!!" );
    
    timerMutex.lock();
    int timer = _nextTimer;
    _nextTimer += 1;
    std::cerr << "startTimer() => " << timer << ": " << std::time(nullptr) << std::endl;
    timerMutex.unlock();
    _begins[timer] = std::chrono::high_resolution_clock::now();
    _startTimes[timer] = std::time(nullptr);

    return timer;    
}

long getDuration( int timer ) {
    auto end = std::chrono::high_resolution_clock::now();
    auto begin = _begins[timer];

    timerMutex.lock();
    std::cerr << "getDuration(" << timer << "): " << std::time(nullptr) << std::endl;
    timerMutex.unlock();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
}

time_t getTime( int timer ) {
    return _startTimes[timer];
}

void endTimer( int timer, std::string node ) {
    long duration_ms = getDuration(timer);
    
    if (displayTimer()) {
        timerMutex.lock();
        std::cerr << node << "[" << timer << "]: " << duration_ms << " ms" << std::endl;
        //std::cerr << node << "[" << timer << "]: start = " << getTime(timer) << std::endl;
        //std::cerr << node << "[" << timer << "]: end   = " << std::time(nullptr) << std::endl;
        timerMutex.unlock();
    }
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
