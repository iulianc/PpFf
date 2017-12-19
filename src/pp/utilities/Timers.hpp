#ifndef TIMERS_HPP
#define TIMERS_HPP

#include<chrono>

int nextTimer = 0;

std::chrono::high_resolution_clock::time_point begins[100];

int startTimer() {
    int timer = nextTimer;
    nextTimer += 1;
    begins[timer] = std::chrono::high_resolution_clock::now();

    return timer;    
}

long stopTimer( int timer ) {
    auto end = std::chrono::high_resolution_clock::now();
    auto begin = begins[timer];

    return std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
}


#endif
