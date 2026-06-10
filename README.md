Java to C++ converter and SDK

With STL and C++ 17 moving from java code base to C++ becomes easier
By implementing all java class hiearchy and interfaces (virtual classes in c++). 
With the help of python converter, moving to c++ from java is easy.

The idea is to make the interface and c++ class act exactly or close to java.
With the help of converter and macros. you can convert applications to run as c++.
Removing garbage collector and JIT.  the use of smart pointers is required as a pattern.
Otherwise memory leaks would ensue.

Prereq:
pip install javalang - used for Abstract tree anaylsis of java source
C++17 compiler with all the C++17 abilities

No open source libraries used under the hood to keep from dependecies across platforms.
So if adding new capabilites to jxx, you will need to implement from ground up.  Cannot include boost
libraries.  Only stl, clib can be used.  You can reimplement a behavior required using those.
The dependecy tree can become large and changes can have a ripple effect.  That is the reason to
implement from group up.

With that said sometimes it is too much work to build from ground up.  But packages needed are scruiized for 
license compatibility and amount of work to include.  Mainly use MIT license.


Pre-req:
- must be MIT compatibility license (for 3rdparty)
- must have a cmake to easily automate build for jxx
- must test via facade wrapper around jxx classes (jxx.<package>.<class> -> uses in private methods <library added>
- public interface must be jxx constraints (derived from Object - memeory deletion, use jxx::Ptr<type> for returns and const jxx::Ptr<type> for parameters)  

For windows:
Need to create an environment variable:   JXX_TIME_ZONE_DATA
Then use provided zoneinfo.tar.gz (from ubuntu linux timezone /usr/shared/zoneinfo)
This provides windows with timezone data.
Also create a file in JXX_TIME_ZONE_DATA/timezone
Set inside file the correct timezone.  Default is GMT
If using linux based systems timezone is already in /usr/shared/zoneinfo


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

When developing additional SDK class / interfaces and methods please do the following:
Make one class per pull request
Make sure a unit test exists in the pull request for new classes
Make sure a benchmark exist and class is same overall time as java or better (if its poor performance, please fix before submitting)


# Note on pull request requirements:
1) Must include one class update / new with interfaces (if applicable)
2) Must include a googletest unit test for all methods and code coverage for the method/function
3) If an algorithm class, must include bench mark test

This will help with proving semantics.  If possible find the java 8 SDK unit test in java and use python converter to convert.  This really helps tests semantics



Other options are


# convert a folder and subfolder to c++ output mimic folder
# use NEW instead of JXX_NEW for similar to java new / c++ new
# include header guards (#ifndef)
python3 java_to_cpp.py --dir ./src_java --out ./cpp_out --cmake \
  --new-macro-style template --new-macro NEW \
  --header-guards \
  --string-include "String.h" \
  --bytearray-include "ByteArray.h" \
  --exceptions-include "Exception.h" \
  --sync-include "Sync.h" \
  --checked-exceptions macro --throws-macro JXX_THROWS





python3 java_to_cpp.py \
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
  
  
  
  
  python3 java2cpp17.py --dir ./src_java --out ./cpp_out \
  --primitive-map int=jint \
  --primitive-map boolean=jbool \
  --primitive-map short=jshort

python3 java_to_cpp.py MyFile.java --instanceof-style macro --downcast-style macro

python3 java_to_cpp.py --dir src_java --out outcpp --cmake



To build 3rd party packages
These 3rd party libraries have been modified to support C++17 at a minimum

top level directory do these commands
git submodule add --name libunicode https://github.com/OpenByteSotware/libunicode 3rdparty/libunicode
git submodule add --name libgsl https://github.com/OpenByteSotware/GSL 3rdparty/gsl
git submodule add --name libfmt https://github.com/OpenByteSotware/fmt 3rdparty/fmt

