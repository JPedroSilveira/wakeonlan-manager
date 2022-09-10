#ifndef MANAGER_NOT_FOUND_EXCEPTION_HEADER
#define MANAGER_NOT_FOUND_EXCEPTION_HEADER

#include <iostream>

struct ManagerNotFoundException : public std::exception 
{
   const char * what () const throw ();
};

#endif