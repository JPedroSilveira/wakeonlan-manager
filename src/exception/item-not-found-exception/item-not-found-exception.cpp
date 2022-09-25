#include "./item-not-found-exception.h"

const char * ItemNotFoundException::what () const throw () {
    return "Item not found";
}