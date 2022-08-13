#include "./not-alive-exception.h"

const char * NotAliveException::what () const throw () {
    return "Application is not alive anymore";
}