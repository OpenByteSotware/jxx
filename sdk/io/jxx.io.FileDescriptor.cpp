
#include "io/jxx.io.FileDescriptor.h"
namespace jxx { namespace io {
FileDescriptor FileDescriptor::in(){ return FileDescriptor(Kind::STDIN); }
FileDescriptor FileDescriptor::out(){ return FileDescriptor(Kind::STDOUT); }
FileDescriptor FileDescriptor::err(){ return FileDescriptor(Kind::STDERR); }
}} // ns
