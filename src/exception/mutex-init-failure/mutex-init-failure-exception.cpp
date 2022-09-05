#include "./mutex-init-failure-exception.h"

const char * MutexInitFailureException::what () const throw () {
    return "Fail to create mutex";
}