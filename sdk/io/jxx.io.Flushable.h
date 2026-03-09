#include <iostream>
#include <string>
#include <stdexcept>

namespace jxx::io {
// Interface-like structure in C++17 using a pure abstract class
class Flushable {
public:
    virtual ~Flushable() = default; // Always provide a virtual destructor for interfaces

    // Pure virtual method (must be overridden by derived classes)
    virtual void flush() = 0;
};
}