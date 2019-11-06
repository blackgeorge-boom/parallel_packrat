//
// Created by blackgeorge on 11/6/19.
//

#ifndef PARALLEL_PACKRAT_STOPPABLE_H
#define PARALLEL_PACKRAT_STOPPABLE_H

#include <thread>
#include <iostream>
#include <chrono>
#include <future>

class Stoppable {
    std::promise<void> exitSignal;
    std::future<void> futureObj;
public:
    Stoppable() : futureObj(exitSignal.get_future()) {}

    Stoppable(Stoppable&& obj);

    Stoppable& operator=(Stoppable&& obj);

    // Task need to provide definition  for this function
    // It will be called by thread function
    virtual void run() = 0;

    // Thread function to be executed by thread
    void operator()() { run(); }

    //Checks if thread is requested to stop
    bool stopRequested();

    // Request the thread to stop by setting value in promise object
    void stop() { exitSignal.set_value(); }
};

#endif //PARALLEL_PACKRAT_STOPPABLE_H
