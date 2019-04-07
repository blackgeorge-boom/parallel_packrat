//
// Created by blackgeorge on 3/12/19.
//

#include <iostream>

#include "cell.h"

std::ostream& operator<<(std::ostream& os, const Cell& c)
{
    os << "{";
    switch (c.res()) {
        case Result::unknown:
            os << "U";
            break;
        case Result::fail:
            os << "F";
            break;
        case Result::success:
            os << "S";
            break;
        default:
            os << "Something's wrong";
    }
    os << "," << c.pos() << "}";
    if (c.pos() != -1)  // just for formatted printing
        os << " ";
    return os;
}


