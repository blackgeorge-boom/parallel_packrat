//
// Created by blackgeorge on 11/6/19.
//

#include "stoppable.h"

Stoppable::Stoppable(Stoppable&& obj) noexcept
    : exitSignal(std::move(obj.exitSignal)), futureObj(std::move(obj.futureObj))
{
}

Stoppable& Stoppable::operator=(Stoppable&& obj) noexcept
{
    exitSignal = std::move(obj.exitSignal);
    futureObj = std::move(obj.futureObj);
    return *this;
}

bool Stoppable::stopRequested()
{
    std::chrono::milliseconds span {0};

    // checks if value in future object is available
    return !(futureObj.wait_for(span) ==
             std::future_status::timeout);
}


