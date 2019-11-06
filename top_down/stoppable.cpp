//
// Created by blackgeorge on 11/6/19.
//

#include "stoppable.h"

Stoppable::Stoppable(Stoppable&& obj)
    : exitSignal(std::move(obj.exitSignal)), futureObj(std::move(obj.futureObj))
{
    std::cout << "Move Constructor is called" << std::endl;
}

Stoppable& Stopable::operator=(Stoppable&& obj)
{
    std::cout << "Move Assignment is called" << std::endl;
    exitSignal = std::move(obj.exitSignal);
    futureObj = std::move(obj.futureObj);
    return *this;
}

bool Stoppable::stopRequested()
{
    // checks if value in future object is available
    return !(futureObj.wait_for(std::chrono::milliseconds(0)) ==
             std::future_status::timeout);
}


