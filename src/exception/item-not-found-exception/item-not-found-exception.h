#ifndef ITEM_NOT_FOUND_EXCEPTION_HEADER
#define ITEM_NOT_FOUND_EXCEPTION_HEADER

#include <iostream>

struct ItemNotFoundException : public std::exception 
{
   const char * what () const throw ();
};

#endif