#ifndef NOT_ALIVE_EXCEPTION_HEADER
#define NOT_ALIVE_EXCEPTION_HEADER

#include <iostream>

struct NotAliveException : public std::exception 
{
   const char * what () const throw ();
};

#endif