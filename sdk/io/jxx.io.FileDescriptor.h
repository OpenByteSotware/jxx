
#pragma once

namespace jxx { namespace io {
class FileDescriptor {
public:
    enum Kind { STDIN, STDOUT, STDERR };
private:
    Kind kind_;
    explicit FileDescriptor(Kind k): kind_(k) {}
public:
    static FileDescriptor in();
    static FileDescriptor out();
    static FileDescriptor err();
    Kind kind() const { return kind_; }
};
}}
