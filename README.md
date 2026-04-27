Java to C++ converter and SDK

With STL and C++ 17 moving from java code base to C++ becomes easier
By implementing all java class hiearchy and interfaces (virtual classes in c++). 
And with the help of python converter, moving to c++ from java is easy.

The idea is to make the interface and c++ class act exactly or close to java.
With the help of converter and macros. you can convert applications to run as c++.
Removing garbage collector and JIT.  the use of smart pointers is required as a pattern.
Otherwise memory leaks would ensue.

Prereq:
pip install javalang - used for Abstract tree anaylsis of java source
C++17 compiler with all the C++17 abilities


There are two parts to this project:

1) Java to C++ converter (python)
2) C++ Java SDK - JXX (c++)

The JXX is a mimic (as close as possible) to java.
Thread class in Java is Thread class in c++ (namespace jxx::lang)
All non GUI classes are mapped and semantically the same as java.  So that when java code is converted it should run out of the box with no modifications.
There are some caviats like serialization and thread priority.  But if an application just runs with queues and threads, there should be no refactor of the C++ generated code.

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




# Note on pull request requirements:
1) Must include one class update / new with interfaces (if applicable)
2) Must include a googletest unit test for all methods and code coverage for the method/function
3) If an algorithm class, must include bench mark test

This will help with proving semantics.  If possible find the java 8 SDK unit test in java and use python converter to convert.  This really helps tests semantics



Other options are


# convert a folder and subfolder to c++ output mimic folder
# use NEW instead of JXX_NEW for similar to java new / c++ new
# include header guards (#ifndef)
python java_to_cpp.py --dir ./src_java --out ./cpp_out --cmake \
  --new-macro-style template --new-macro NEW \
  --header-guards \
  --string-include "String.h" \
  --bytearray-include "ByteArray.h" \
  --exceptions-include "Exception.h" \
  --sync-include "Sync.h" \
  --checked-exceptions macro --throws-macro JXX_THROWS





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
  --checked-exceptions macro --throws-macro JXX_THROWS
  
  
  
  
  python java2cpp17.py --dir ./src_java --out ./cpp_out \
  --primitive-map int=jint \
  --primitive-map boolean=jbool \
  --primitive-map short=jshort

python java_to_cpp.py MyFile.java --instanceof-style macro --downcast-style macro

python java_to_cpp.py --dir src_java --out outcpp --cmake


