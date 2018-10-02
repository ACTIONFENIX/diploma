#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <string>

class ErrorHandler
{
public:
    enum handling_type{message, exception};
    handling_type type;
    static int calls;

public:
    ErrorHandler(const std::string& msg, handling_type t = handling_type::message);
};

#endif //ERRORHANDLER_H

