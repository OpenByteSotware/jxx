Java to C++ converter and SDK

With STL and C++ 17 moving from java code base to C++ becomes easier
By implementing all java class hiearchy and interfaces (virtual classes in c++). 
And with the help of python converter, moving to c++ from java is easy.

The idea is to make the interface and c++ class act exactly or close to java.
With the help of converter and macros. you can convert applications to run as c++.
Removing garbage collector and JIT.  the use of smart pointers is required as a pattern.
Otherwise memory leaks would ensue.

There are two parts to this project:

java to cpp converter (python)
and the C++ Java SDK (jxx)


The python converter will take a single file or directory structure and convert every .java file to a .h and .cpp with same name
And can optionally generate a cmake file to build your new converted project.
Here are cmake options
Pass --cmake in directory or single-file pair mode and a CMakeLists.txt is created at --out:
Options

--cmake-project TranspiledProject
--cmake-target transpiled
--cmake-kind {static|shared|object|interface} (default static)
--cmake-cxx-standard 17 (string value)
--cmake-min 3.20 (CMake minimum version)






Other options are

python java_to_cpp.py \
  --dir ./src_java \
  --out ./cpp_out \
  --cmake \
  --cmake-project MyTranspiled \
  --cmake-target mylib \
  --cmake-kind static \
  --cmake-cxx-standard 17 \
  --string-include "String.hpp" \
  --bytearray-include "ByteArray.hpp" \
  --exceptions-include "Exception.hpp" \
  --exceptions-include "IOException.hpp" \
  --sync-include "Sync.hpp" \
  --checked-exceptions macro --throws-macro JTHROWS
