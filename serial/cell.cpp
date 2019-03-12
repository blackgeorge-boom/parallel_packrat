//
// Created by blackgeorge on 3/12/19.
//

#include <iostream>

#include "cell.h"

std::ostream& operator<<(std::ostream& os, const Cell& c)
{
    os << "{\"";
    switch (c.res()) {
        case Result::unknown:
            os << "UNKNOWN";
            break;
        case Result::fail:
            os << "FAIL";
            break;
        case Result::success:
            os << "SUCCESS";
            break;
        default:
            os << "Something's wrong";
    }
    return os << "\"," << c.pos() << "}";
}


