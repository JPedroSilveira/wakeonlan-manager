#ifndef ELECTION_EXCEPTION_HEADER
#define ELECTION_EXCEPTION_HEADER

#include <iostream>

struct ElectionException : public std::exception 
{
   const char * what () const throw ();
};

#endif