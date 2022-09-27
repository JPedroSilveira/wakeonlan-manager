#include "./boolean-utils.h"

std::string booleanToString(bool value) {
    return value ? "t" : "f";
}

bool stringToBoolean(std::string value) {
    return value == "t";
}