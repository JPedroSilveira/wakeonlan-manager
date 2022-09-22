#include "./election-exception.h"

const char * ElectionException::what () const throw () {
    return "Failure while doing election";
}