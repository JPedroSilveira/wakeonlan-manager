#ifndef FATAL_ERROR_EXCEPTION_HEADER
#define FATAL_ERROR_EXCEPTION_HEADER

#include <iostream>

struct FatalErrorException : public std::exception 
{
   const char * what () const throw ();
};

#endif