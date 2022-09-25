#include "./fatal-error-exception.h"

const char * FatalErrorException::what () const throw () {
    return "Non-recoverable error";
}