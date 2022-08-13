#ifndef MUTEX_INIT_FAILURE_EXCEPTION_HEADER
#define MUTEX_INIT_FAILURE_EXCEPTION_HEADER

#include <iostream>

struct MutexInitFailureException : public std::exception 
{
   const char * what () const throw ();
};

#endif