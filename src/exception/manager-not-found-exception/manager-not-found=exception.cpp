#include "./manager-not-found-exception.h"

const char * ManagerNotFoundException::what () const throw () {
    return "Manager not found";
}